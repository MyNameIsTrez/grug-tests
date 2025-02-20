//// GRUG DOCUMENTATION
//
// See the bottom of this file for the MIT license
//
// See [my blog post](https://mynameistrez.github.io/2024/02/29/creating-the-perfect-modding-language.html) for an introduction to the grug modding language.
//
// You can find its test suite [here](https://github.com/MyNameIsTrez/grug-tests).
//
// ## Sections
//
// This file is composed of sections, which you can jump between by searching for `////` in the file:
//
// 1. GRUG DOCUMENTATION
// 2. INCLUDES AND DEFINES
// 3. UTILS
// 4. RUNTIME ERROR HANDLING
// 5. JSON
// 6. PARSING MOD API JSON
// 7. READING
// 8. TOKENIZATION
// 9. PARSING
// 10. DUMPING AST
// 11. APPLYING AST
// 12. TYPE PROPAGATION
// 13. COMPILING
// 14. LINKING
// 15. HOT RELOADING
//
// ## Programs showcasing grug
//
// - [Modding Minecraft with grug](https://github.com/MyNameIsTrez/grug-toys)
// - [Box2D and raylib game](https://github.com/MyNameIsTrez/grug-box2d-and-raylib-game)
// - [terminal game: C/C++](https://github.com/MyNameIsTrez/grug-terminal-game-c-cpp)
// - [terminal game: Python](https://github.com/MyNameIsTrez/grug-terminal-game-python)
// - [terminal game: Java](https://github.com/MyNameIsTrez/grug-terminal-game-java)
// - [grug benchmarks](https://github.com/MyNameIsTrez/grug-benchmarks)
//
// ## Settings
//
// Search for `#define` in this file (with Ctrl+F). All the defines are configurable.
//
// If you want to allow your compiler to optimize this file extra hard, add the compiler flag `-DCRASH_ON_UNREACHABLE`.

//// INCLUDES AND DEFINES

#define _XOPEN_SOURCE 700 // This is just so VS Code can find CLOCK_PROCESS_CPUTIME_ID

#include "grug.h"

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// "The problem is that you can't meaningfully define a constant like this
// in a header file. The maximum path size is actually to be something
// like a filesystem limitation, or at the very least a kernel parameter.
// This means that it's a dynamic value, not something preordained."
// https://eklitzke.org/path-max-is-tricky
#define STUPID_MAX_PATH 4096

static bool streq(char *a, char *b);

#define grug_error(...) {\
	if (snprintf(grug_error.msg, sizeof(grug_error.msg), __VA_ARGS__) < 0) {\
		abort();\
	}\
	\
	grug_error.grug_c_line_number = __LINE__;\
	\
	grug_error.has_changed =\
		!streq(grug_error.msg, previous_grug_error.msg)\
	 || !streq(grug_error.path, previous_grug_error.path)\
	 || grug_error.grug_c_line_number != previous_grug_error.grug_c_line_number;\
	\
	memcpy(previous_grug_error.msg, grug_error.msg, sizeof(grug_error.msg));\
	memcpy(previous_grug_error.path, grug_error.path, sizeof(grug_error.path));\
	previous_grug_error.grug_c_line_number = grug_error.grug_c_line_number;\
	\
	longjmp(error_jmp_buffer, 1);\
}

#define grug_assert(condition, ...) {\
	if (!(condition)) {\
		grug_error(__VA_ARGS__);\
	}\
}

#ifdef CRASH_ON_UNREACHABLE
#define grug_unreachable() {\
	assert(false && "This line of code is supposed to be unreachable. Please report this bug to the grug developers!");\
}
#else
#define grug_unreachable() {\
	grug_error("This line of code in grug.c:%d is supposed to be unreachable. Please report this bug to the grug developers!", __LINE__);\
}
#endif

#ifdef LOGGING
#define grug_log(...) fprintf(stderr, __VA_ARGS__)
#else
#define grug_log(...) {\
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wunused-value\"")\
	__VA_ARGS__;\
	_Pragma("GCC diagnostic pop")\
}
#endif

#define USED_BY_MODS
#define USED_BY_PROGRAMS

#define BFD_HASH_BUCKET_SIZE 4051 // From https://sourceware.org/git/?p=binutils-gdb.git;a=blob;f=bfd/hash.c#l345

//// UTILS

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;

struct grug_error grug_error;
bool grug_loading_error_in_grug_file;
struct grug_error previous_grug_error;
static jmp_buf error_jmp_buffer;

static char global_mods_dir_path[STUPID_MAX_PATH];

static bool streq(char *a, char *b) {
	return strcmp(a, b) == 0;
}

// "a" is the haystack, "b" is the needle
static bool starts_with(char *a, char *b) {
	return strncmp(a, b, strlen(b)) == 0;
}

// "a" is the haystack, "b" is the needle
static bool ends_with(char *a, char *b) {
	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	if (len_a < len_b) {
		return false;
	}
	return strncmp(a + len_a - len_b, b, len_b) == 0;
}

static bool is_lowercase(char *str) {
	for (; *str; str++) {
		// `!islower()` doesn't work, since '-' isn't considered lowercase
		if (isupper(*str)) {
			return false;
		}
	}
	return true;
}

// From https://sourceware.org/git/?p=binutils-gdb.git;a=blob;f=bfd/elf.c#l193
static u32 elf_hash(const char *namearg) {
	u32 h = 0;

	for (const unsigned char *name = (const unsigned char *) namearg; *name; name++) {
		h = (h << 4) + *name;
		h ^= (h >> 24) & 0xf0;
	}

	return h & 0x0fffffff;
}

// This is solely here to put the symbols in the same weird order as ld does
// From https://sourceware.org/git/?p=binutils-gdb.git;a=blob;f=bfd/hash.c#l508
static unsigned long bfd_hash(const char *string) {
	const unsigned char *s;
	unsigned long hash;
	unsigned int len;
	unsigned int c;

	hash = 0;
	s = (const unsigned char *) string;
	while ((c = *s++) != '\0') {
		hash += c + (c << 17);
		hash ^= hash >> 2;
	}
	len = (s - (const unsigned char *) string) - 1;
	hash += len + (len << 17);
	hash ^= hash >> 2;
	return hash;
}

static char *get_file_extension(char *filename) {
	char *ext = strrchr(filename, '.');
	if (ext) {
		return ext;
	}
	return "";
}

static void print_dlerror(char *function_name) {
	char *err = dlerror();
	grug_assert(err, "dlerror() was asked to find an error string, but it couldn't find one");
	grug_error("%s: %s", function_name, err);
}

static void *get_dll_symbol(void *dll, char *symbol_name) {
	return dlsym(dll, symbol_name);
}

//// RUNTIME ERROR HANDLING

#define GRUG_ON_FN_TIME_LIMIT_MS 10
static_assert(GRUG_ON_FN_TIME_LIMIT_MS >= 1 && GRUG_ON_FN_TIME_LIMIT_MS <= 999, "This simplifies the generated machine code");

USED_BY_MODS jmp_buf grug_runtime_error_jmp_buffer;

USED_BY_MODS grug_runtime_error_handler_t grug_runtime_error_handler = NULL;

USED_BY_MODS char *grug_get_runtime_error_reason(enum grug_runtime_error_type type);

char *grug_get_runtime_error_reason(enum grug_runtime_error_type type) {
	switch (type) {
		case GRUG_ON_FN_DIVISION_BY_ZERO:
			return "Division of an i32 by 0";
		case GRUG_ON_FN_STACK_OVERFLOW:
			return "Stack overflow, so check for accidental infinite recursion";
		case GRUG_ON_FN_TIME_LIMIT_EXCEEDED: {
			static char temp[420];

			snprintf(temp, sizeof(temp), "Took longer than %d milliseconds to run", GRUG_ON_FN_TIME_LIMIT_MS);

			return temp;
		}
		case GRUG_ON_FN_OVERFLOW:
			return "i32 overflow";
	}
	grug_unreachable();
}

//// JSON

#define JSON_MAX_CHARACTERS 420420
#define JSON_MAX_TOKENS 420420
#define JSON_MAX_NODES 420420
#define JSON_MAX_FIELDS 420420
#define JSON_MAX_CHILD_NODES 1337
#define JSON_MAX_STRINGS_CHARACTERS 420420
#define JSON_MAX_RECURSION_DEPTH 42

#define json_error(error) {\
	grug_error("JSON error: %s: %s", json_file_path, json_error_messages[error]);\
}

#define json_assert(condition, error) {\
	if (!(condition)) {\
		json_error(error);\
	}\
}

struct json_array {
	struct json_node *values;
	size_t value_count;
};

struct json_object {
	struct json_field *fields;
	size_t field_count;
};

struct json_field {
	char *key;
	struct json_node *value;
};

struct json_node {
	enum {
		JSON_NODE_STRING,
		JSON_NODE_ARRAY,
		JSON_NODE_OBJECT,
	} type;
	union {
		char *string;
		struct json_array array;
		struct json_object object;
	};
};

enum json_error {
	JSON_NO_ERROR,
	JSON_ERROR_FAILED_TO_OPEN_FILE,
	JSON_ERROR_FAILED_TO_CLOSE_FILE,
	JSON_ERROR_FILE_EMPTY,
	JSON_ERROR_FILE_TOO_BIG,
	JSON_ERROR_FILE_READING_ERROR,
	JSON_ERROR_UNRECOGNIZED_CHARACTER,
	JSON_ERROR_UNCLOSED_STRING,
	JSON_ERROR_DUPLICATE_KEY,
	JSON_ERROR_TOO_MANY_TOKENS,
	JSON_ERROR_TOO_MANY_NODES,
	JSON_ERROR_TOO_MANY_FIELDS,
	JSON_ERROR_TOO_MANY_CHILD_NODES,
	JSON_ERROR_MAX_RECURSION_DEPTH_EXCEEDED,
	JSON_ERROR_TRAILING_COMMA,
	JSON_ERROR_EXPECTED_ARRAY_CLOSE,
	JSON_ERROR_EXPECTED_OBJECT_CLOSE,
	JSON_ERROR_EXPECTED_COLON,
	JSON_ERROR_EXPECTED_VALUE,
	JSON_ERROR_UNEXPECTED_STRING,
	JSON_ERROR_UNEXPECTED_ARRAY_OPEN,
	JSON_ERROR_UNEXPECTED_ARRAY_CLOSE,
	JSON_ERROR_UNEXPECTED_OBJECT_OPEN,
	JSON_ERROR_UNEXPECTED_OBJECT_CLOSE,
	JSON_ERROR_UNEXPECTED_COMMA,
	JSON_ERROR_UNEXPECTED_COLON,
	JSON_ERROR_UNEXPECTED_EXTRA_CHARACTER,
};

static char *json_error_messages[] = {
	[JSON_NO_ERROR] = "No error",
	[JSON_ERROR_FAILED_TO_OPEN_FILE] = "Failed to open file",
	[JSON_ERROR_FAILED_TO_CLOSE_FILE] = "Failed to close file",
	[JSON_ERROR_FILE_EMPTY] = "File is empty",
	[JSON_ERROR_FILE_TOO_BIG] = "File is too big",
	[JSON_ERROR_FILE_READING_ERROR] = "File reading error",
	[JSON_ERROR_UNRECOGNIZED_CHARACTER] = "Unrecognized character",
	[JSON_ERROR_UNCLOSED_STRING] = "Unclosed string",
	[JSON_ERROR_DUPLICATE_KEY] = "Duplicate key",
	[JSON_ERROR_TOO_MANY_TOKENS] = "Too many tokens",
	[JSON_ERROR_TOO_MANY_NODES] = "Too many nodes",
	[JSON_ERROR_TOO_MANY_FIELDS] = "Too many fields",
	[JSON_ERROR_TOO_MANY_CHILD_NODES] = "Too many child nodes",
	[JSON_ERROR_MAX_RECURSION_DEPTH_EXCEEDED] = "Max recursion depth exceeded",
	[JSON_ERROR_TRAILING_COMMA] = "Trailing comma",
	[JSON_ERROR_EXPECTED_ARRAY_CLOSE] = "Expected ']'",
	[JSON_ERROR_EXPECTED_OBJECT_CLOSE] = "Expected '}'",
	[JSON_ERROR_EXPECTED_COLON] = "Expected colon",
	[JSON_ERROR_EXPECTED_VALUE] = "Expected value",
	[JSON_ERROR_UNEXPECTED_STRING] = "Unexpected string",
	[JSON_ERROR_UNEXPECTED_ARRAY_OPEN] = "Unexpected '['",
	[JSON_ERROR_UNEXPECTED_ARRAY_CLOSE] = "Unexpected ']'",
	[JSON_ERROR_UNEXPECTED_OBJECT_OPEN] = "Unexpected '{'",
	[JSON_ERROR_UNEXPECTED_OBJECT_CLOSE] = "Unexpected '}'",
	[JSON_ERROR_UNEXPECTED_COMMA] = "Unexpected ','",
	[JSON_ERROR_UNEXPECTED_COLON] = "Unexpected ':'",
	[JSON_ERROR_UNEXPECTED_EXTRA_CHARACTER] = "Unexpected extra character",
};

static char *json_file_path;

static size_t json_recursion_depth;

static char json_text[JSON_MAX_CHARACTERS];
static size_t json_text_size;

enum json_token_type {
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_ARRAY_OPEN,
	TOKEN_TYPE_ARRAY_CLOSE,
	TOKEN_TYPE_OBJECT_OPEN,
	TOKEN_TYPE_OBJECT_CLOSE,
	TOKEN_TYPE_COMMA,
	TOKEN_TYPE_COLON,
};

struct json_token {
	enum json_token_type type;
	char *str;
};
static struct json_token json_tokens[JSON_MAX_TOKENS];
static size_t json_tokens_size;

static struct json_node json_nodes[JSON_MAX_NODES];
static size_t json_nodes_size;

static struct json_field json_fields[JSON_MAX_FIELDS];
static size_t json_fields_size;

static u32 json_buckets[JSON_MAX_FIELDS];
static u32 json_chains[JSON_MAX_FIELDS];

static char json_strings[JSON_MAX_STRINGS_CHARACTERS];
static size_t json_strings_size;

static struct json_node json_parse_string(size_t *i);
static struct json_node json_parse_array(size_t *i);

static void json_push_node(struct json_node node) {
	json_assert(json_nodes_size < JSON_MAX_NODES, JSON_ERROR_TOO_MANY_NODES);
	json_nodes[json_nodes_size++] = node;
}

static void json_push_field(struct json_field field) {
	json_assert(json_fields_size < JSON_MAX_FIELDS, JSON_ERROR_TOO_MANY_FIELDS);
	json_fields[json_fields_size++] = field;
}

static bool is_duplicate_key(struct json_field *child_fields, size_t field_count, char *key) {
	u32 i = json_buckets[elf_hash(key) % field_count];

	while (true) {
		if (i == UINT32_MAX) {
			return false;
		}

		if (streq(key, child_fields[i].key)) {
			break;
		}

		i = json_chains[i];
	}

	return true;
}

static void check_duplicate_keys(struct json_field *child_fields, size_t field_count) {
	memset(json_buckets, 0xff, field_count * sizeof(u32));

	for (size_t i = 0; i < field_count; i++) {
		char *key = child_fields[i].key;

		json_assert(!is_duplicate_key(child_fields, field_count, key), JSON_ERROR_DUPLICATE_KEY);

		u32 bucket_index = elf_hash(key) % field_count;

		json_chains[i] = json_buckets[bucket_index];

		json_buckets[bucket_index] = i;
	}
}

static struct json_node json_parse_object(size_t *i) {
	struct json_node node;

	node.type = JSON_NODE_OBJECT;
	(*i)++;

	json_recursion_depth++;
	json_assert(json_recursion_depth <= JSON_MAX_RECURSION_DEPTH, JSON_ERROR_MAX_RECURSION_DEPTH_EXCEEDED);

	node.object.field_count = 0;

	struct json_field child_fields[JSON_MAX_CHILD_NODES];

	bool seen_key = false;
	bool seen_colon = false;
	bool seen_value = false;
	bool seen_comma = false;

	struct json_field field;

	struct json_node string;
	struct json_node array;
	struct json_node object;

	while (*i < json_tokens_size) {
		struct json_token *token = json_tokens + *i;

		switch (token->type) {
		case TOKEN_TYPE_STRING:
			if (!seen_key) {
				seen_key = true;
				field.key = token->str;
				(*i)++;
			} else if (seen_colon && !seen_value) {
				seen_value = true;
				seen_comma = false;
				string = json_parse_string(i);
				field.value = json_nodes + json_nodes_size;
				json_push_node(string);
				json_assert(node.object.field_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
				child_fields[node.object.field_count++] = field;
			} else {
				json_error(JSON_ERROR_UNEXPECTED_STRING);
			}
			break;
		case TOKEN_TYPE_ARRAY_OPEN:
			if (seen_colon && !seen_value) {
				seen_value = true;
				seen_comma = false;
				array = json_parse_array(i);
				field.value = json_nodes + json_nodes_size;
				json_push_node(array);
				json_assert(node.object.field_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
				child_fields[node.object.field_count++] = field;
			} else {
				json_error(JSON_ERROR_UNEXPECTED_ARRAY_OPEN);
			}
			break;
		case TOKEN_TYPE_ARRAY_CLOSE:
			json_error(JSON_ERROR_UNEXPECTED_ARRAY_CLOSE);
		case TOKEN_TYPE_OBJECT_OPEN:
			if (seen_colon && !seen_value) {
				seen_value = true;
				seen_comma = false;
				object = json_parse_object(i);
				field.value = json_nodes + json_nodes_size;
				json_push_node(object);
				json_assert(node.object.field_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
				child_fields[node.object.field_count++] = field;
			} else {
				json_error(JSON_ERROR_UNEXPECTED_OBJECT_OPEN);
			}
			break;
		case TOKEN_TYPE_OBJECT_CLOSE:
			if (seen_key && !seen_colon) {
				json_error(JSON_ERROR_EXPECTED_COLON);
			} else if (seen_colon && !seen_value) {
				json_error(JSON_ERROR_EXPECTED_VALUE);
			} else if (seen_comma) {
				json_error(JSON_ERROR_TRAILING_COMMA);
			}
			check_duplicate_keys(child_fields, node.object.field_count);
			node.object.fields = json_fields + json_fields_size;
			for (size_t field_index = 0; field_index < node.object.field_count; field_index++) {
				json_push_field(child_fields[field_index]);
			}
			(*i)++;
			json_recursion_depth--;
			return node;
		case TOKEN_TYPE_COMMA:
			json_assert(seen_value, JSON_ERROR_UNEXPECTED_COMMA);
			seen_key = false;
			seen_colon = false;
			seen_value = false;
			seen_comma = true;
			(*i)++;
			break;
		case TOKEN_TYPE_COLON:
			json_assert(seen_key, JSON_ERROR_UNEXPECTED_COLON);
			seen_colon = true;
			(*i)++;
			break;
		}
	}

	json_error(JSON_ERROR_EXPECTED_OBJECT_CLOSE);
}

static struct json_node json_parse_array(size_t *i) {
	struct json_node node;

	node.type = JSON_NODE_ARRAY;
	(*i)++;

	json_recursion_depth++;
	json_assert(json_recursion_depth <= JSON_MAX_RECURSION_DEPTH, JSON_ERROR_MAX_RECURSION_DEPTH_EXCEEDED);

	node.array.value_count = 0;

	struct json_node child_nodes[JSON_MAX_CHILD_NODES];

	bool seen_value = false;
	bool seen_comma = false;

	while (*i < json_tokens_size) {
		struct json_token *token = json_tokens + *i;

		switch (token->type) {
		case TOKEN_TYPE_STRING:
			json_assert(!seen_value, JSON_ERROR_UNEXPECTED_STRING);
			seen_value = true;
			seen_comma = false;
			json_assert(node.array.value_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
			child_nodes[node.array.value_count++] = json_parse_string(i);
			break;
		case TOKEN_TYPE_ARRAY_OPEN:
			json_assert(!seen_value, JSON_ERROR_UNEXPECTED_ARRAY_OPEN);
			seen_value = true;
			seen_comma = false;
			json_assert(node.array.value_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
			child_nodes[node.array.value_count++] = json_parse_array(i);
			break;
		case TOKEN_TYPE_ARRAY_CLOSE:
			json_assert(!seen_comma, JSON_ERROR_TRAILING_COMMA);
			node.array.values = json_nodes + json_nodes_size;
			for (size_t value_index = 0; value_index < node.array.value_count; value_index++) {
				json_push_node(child_nodes[value_index]);
			}
			(*i)++;
			json_recursion_depth--;
			return node;
		case TOKEN_TYPE_OBJECT_OPEN:
			json_assert(!seen_value, JSON_ERROR_UNEXPECTED_OBJECT_OPEN);
			seen_value = true;
			seen_comma = false;
			json_assert(node.array.value_count < JSON_MAX_CHILD_NODES, JSON_ERROR_TOO_MANY_CHILD_NODES);
			child_nodes[node.array.value_count++] = json_parse_object(i);
			break;
		case TOKEN_TYPE_OBJECT_CLOSE:
			json_error(JSON_ERROR_UNEXPECTED_OBJECT_CLOSE);
		case TOKEN_TYPE_COMMA:
			json_assert(seen_value, JSON_ERROR_UNEXPECTED_COMMA);
			seen_value = false;
			seen_comma = true;
			(*i)++;
			break;
		case TOKEN_TYPE_COLON:
			json_error(JSON_ERROR_UNEXPECTED_COLON);
		}
	}

	json_error(JSON_ERROR_EXPECTED_ARRAY_CLOSE);
}

static struct json_node json_parse_string(size_t *i) {
	struct json_node node;

	node.type = JSON_NODE_STRING;

	struct json_token *token = json_tokens + *i;
	node.string = token->str;

	(*i)++;

	return node;
}

static struct json_node json_parse(size_t *i) {
	struct json_token *t = json_tokens + *i;
	struct json_node node;

	switch (t->type) {
	case TOKEN_TYPE_STRING:
		node = json_parse_string(i);
		break;
	case TOKEN_TYPE_ARRAY_OPEN:
		node = json_parse_array(i);
		break;
	case TOKEN_TYPE_ARRAY_CLOSE:
		json_error(JSON_ERROR_UNEXPECTED_ARRAY_CLOSE);
	case TOKEN_TYPE_OBJECT_OPEN:
		node = json_parse_object(i);
		break;
	case TOKEN_TYPE_OBJECT_CLOSE:
		json_error(JSON_ERROR_UNEXPECTED_OBJECT_CLOSE);
	case TOKEN_TYPE_COMMA:
		json_error(JSON_ERROR_UNEXPECTED_COMMA);
	case TOKEN_TYPE_COLON:
		json_error(JSON_ERROR_UNEXPECTED_COLON);
	}

	json_assert(*i >= json_tokens_size, JSON_ERROR_UNEXPECTED_EXTRA_CHARACTER);

	return node;
}

static char *json_push_string(char *slice_start, size_t length) {
	grug_assert(json_strings_size + length < JSON_MAX_STRINGS_CHARACTERS, "There are more than %d characters in the json_strings array, exceeding JSON_MAX_STRINGS_CHARACTERS", JSON_MAX_STRINGS_CHARACTERS);

	char *new_str = json_strings + json_strings_size;

	for (size_t i = 0; i < length; i++) {
		json_strings[json_strings_size++] = slice_start[i];
	}
	json_strings[json_strings_size++] = '\0';

	return new_str;
}

static void json_push_token(enum json_token_type type, size_t offset, size_t length) {
	json_assert(json_tokens_size < JSON_MAX_TOKENS, JSON_ERROR_TOO_MANY_TOKENS);
	json_tokens[json_tokens_size++] = (struct json_token){
		.type = type,
		.str = json_push_string(json_text + offset, length),
	};
}

static void json_tokenize(void) {
	size_t i = 0;

	while (i < json_text_size) {
		if (json_text[i] == '"') {
			size_t string_start_index = i;

			while (++i < json_text_size && json_text[i] != '"') {}

			json_assert(json_text[i] == '"', JSON_ERROR_UNCLOSED_STRING);

			json_push_token(
				TOKEN_TYPE_STRING,
				string_start_index + 1,
				i - string_start_index - 1
			);
		} else if (json_text[i] == '[') {
			json_push_token(TOKEN_TYPE_ARRAY_OPEN, i, 1);
		} else if (json_text[i] == ']') {
			json_push_token(TOKEN_TYPE_ARRAY_CLOSE, i, 1);
		} else if (json_text[i] == '{') {
			json_push_token(TOKEN_TYPE_OBJECT_OPEN, i, 1);
		} else if (json_text[i] == '}') {
			json_push_token(TOKEN_TYPE_OBJECT_CLOSE, i, 1);
		} else if (json_text[i] == ',') {
			json_push_token(TOKEN_TYPE_COMMA, i, 1);
		} else if (json_text[i] == ':') {
			json_push_token(TOKEN_TYPE_COLON, i, 1);
		} else if (!isspace(json_text[i])) {
			json_error(JSON_ERROR_UNRECOGNIZED_CHARACTER);
		}
		i++;
	}
}

static void json_read_text(char *file_path) {
	FILE *f = fopen(file_path, "r");
	if (!f) {
		grug_error("JSON error: %s '%s'", json_error_messages[JSON_ERROR_FAILED_TO_OPEN_FILE], file_path);
	}

	json_text_size = fread(
		json_text,
		sizeof(char),
		JSON_MAX_CHARACTERS,
		f
	);

	int is_eof = feof(f);
	int err = ferror(f);

	json_assert(fclose(f) == 0, JSON_ERROR_FAILED_TO_CLOSE_FILE);

	json_assert(json_text_size != 0, JSON_ERROR_FILE_EMPTY);
	json_assert(is_eof && json_text_size != JSON_MAX_CHARACTERS, JSON_ERROR_FILE_TOO_BIG);
	json_assert(err == 0, JSON_ERROR_FILE_READING_ERROR);

	json_text[json_text_size] = '\0';
}

static void json_reset(void) {
	json_file_path = NULL;
	json_recursion_depth = 0;
	json_text_size = 0;
	json_tokens_size = 0;
	json_nodes_size = 0;
	json_strings_size = 0;
	json_fields_size = 0;
}

static void json(char *file_path, struct json_node *returned) {
	json_reset();

	json_file_path = file_path;

	json_read_text(file_path);

	json_tokenize();

	size_t token_index = 0;
	*returned = json_parse(&token_index);
}

//// PARSING MOD API JSON

#define MAX_GRUG_FUNCTIONS 420420
#define MAX_GRUG_ARGUMENTS 420420

enum type {
	type_void,
	type_bool,
	type_i32,
	type_f32,
	type_string,
	type_id,
	type_resource,
	type_entity,
};
static char *type_names[] = {
	[type_bool] = "bool",
	[type_i32] = "i32",
	[type_f32] = "f32",
	[type_string] = "string",
	[type_id] = "id",
	[type_resource] = "resource",
	[type_entity] = "entity",
};
static size_t type_sizes[] = {
	[type_bool] = sizeof(bool),
	[type_i32] = sizeof(i32),
	[type_f32] = sizeof(float),
	[type_string] = sizeof(char *),
	[type_id] = sizeof(u64),
	[type_resource] = sizeof(char *),
	[type_entity] = sizeof(char *),
};

struct grug_on_function {
	char *name;
	struct argument *arguments;
	size_t argument_count;
};

struct grug_entity {
	char *name;
	struct argument *fields;
	size_t field_count;
	struct grug_on_function *on_functions;
	size_t on_function_count;
};

struct grug_game_function {
	char *name;
	enum type return_type;
	struct argument *arguments;
	size_t argument_count;
};

struct argument {
	char *name;
	enum type type;
	union {
		char *resource_extension; // This is optional
		char *entity_type; // This is optional
	};
};

static struct grug_on_function grug_on_functions[MAX_GRUG_FUNCTIONS];
static size_t grug_on_functions_size;

static struct grug_entity grug_define_functions[MAX_GRUG_FUNCTIONS];
static size_t grug_define_functions_size;

static struct grug_game_function grug_game_functions[MAX_GRUG_FUNCTIONS];
static size_t grug_game_functions_size;
static u32 buckets_game_fns[MAX_GRUG_FUNCTIONS];
static u32 chains_game_fns[MAX_GRUG_FUNCTIONS];

static struct argument grug_arguments[MAX_GRUG_ARGUMENTS];
static size_t grug_arguments_size;

static char mod_api_strings[JSON_MAX_STRINGS_CHARACTERS];
static size_t mod_api_strings_size;

static void push_grug_on_function(struct grug_on_function fn) {
	grug_assert(grug_on_functions_size < MAX_GRUG_FUNCTIONS, "There are more than %d on_ functions in mod_api.json, exceeding MAX_GRUG_FUNCTIONS", MAX_GRUG_FUNCTIONS);
	grug_on_functions[grug_on_functions_size++] = fn;
}

static void push_grug_entity(struct grug_entity fn) {
	grug_assert(grug_define_functions_size < MAX_GRUG_FUNCTIONS, "There are more than %d define functions in mod_api.json, exceeding MAX_GRUG_FUNCTIONS", MAX_GRUG_FUNCTIONS);
	grug_define_functions[grug_define_functions_size++] = fn;
}

static struct grug_game_function *get_grug_game_fn(char *name) {
	if (grug_game_functions_size == 0) {
		return NULL;
	}

	u32 i = buckets_game_fns[elf_hash(name) % grug_game_functions_size];

	while (true) {
		if (i == UINT32_MAX) {
			return NULL;
		}

		if (streq(name, grug_game_functions[i].name)) {
			break;
		}

		i = chains_game_fns[i];
	}

	return grug_game_functions + i;
}

static void hash_game_fns(void) {
	memset(buckets_game_fns, 0xff, grug_game_functions_size * sizeof(u32));

	for (size_t i = 0; i < grug_game_functions_size; i++) {
		char *name = grug_game_functions[i].name;

		u32 bucket_index = elf_hash(name) % grug_game_functions_size;

		chains_game_fns[i] = buckets_game_fns[bucket_index];

		buckets_game_fns[bucket_index] = i;
	}
}

static void push_grug_game_function(struct grug_game_function fn) {
	grug_assert(grug_game_functions_size < MAX_GRUG_FUNCTIONS, "There are more than %d game functions in mod_api.json, exceeding MAX_GRUG_FUNCTIONS", MAX_GRUG_FUNCTIONS);
	grug_game_functions[grug_game_functions_size++] = fn;
}

static void push_grug_argument(struct argument argument) {
	grug_assert(grug_arguments_size < MAX_GRUG_ARGUMENTS, "There are more than %d grug arguments, exceeding MAX_GRUG_ARGUMENTS", MAX_GRUG_ARGUMENTS);
	grug_arguments[grug_arguments_size++] = argument;
}

static char *push_mod_api_string(char *old_str) {
	size_t length = strlen(old_str);

	grug_assert(mod_api_strings_size + length < JSON_MAX_STRINGS_CHARACTERS, "There are more than %d characters in the mod_api_strings array, exceeding JSON_MAX_STRINGS_CHARACTERS", JSON_MAX_STRINGS_CHARACTERS);

	char *new_str = mod_api_strings + mod_api_strings_size;

	memcpy(mod_api_strings + mod_api_strings_size, old_str, length + 1);
	mod_api_strings_size += length + 1;

	return new_str;
}

static enum type parse_type(char *type) {
	if (streq(type, "bool")) {
		return type_bool;
	}
	if (streq(type, "i32")) {
		return type_i32;
	}
	if (streq(type, "f32")) {
		return type_f32;
	}
	if (streq(type, "string")) {
		return type_string;
	}
	if (streq(type, "id")) {
		return type_id;
	}
	if (streq(type, "resource")) {
		return type_resource;
	}
	if (streq(type, "entity")) {
		return type_entity;
	}

	// Make sure to add any new types to this error message
	grug_error("The type '%s' must be changed to one of bool/i32/f32/string/id/resource/entity", type);
}

static void init_game_fns(struct json_object fns) {
	for (size_t fn_index = 0; fn_index < fns.field_count; fn_index++) {
		struct grug_game_function grug_fn = {0};

		grug_fn.name = push_mod_api_string(fns.fields[fn_index].key);
		grug_assert(!streq(grug_fn.name, ""), "\"game_functions\" its function names must not be an empty string");
		grug_assert(!starts_with(grug_fn.name, "on_"), "\"game_functions\" its function names must not start with 'on_'");

		grug_assert(fns.fields[fn_index].value->type == JSON_NODE_OBJECT, "\"game_functions\" its array must only contain objects");
		struct json_object fn = fns.fields[fn_index].value->object;
		grug_assert(fn.field_count >= 1, "\"game_functions\" its objects must have at least a \"description\" field");
		grug_assert(fn.field_count <= 3, "\"game_functions\" its objects must not have more than 3 fields");

		struct json_field *field = fn.fields;

		grug_assert(streq(field->key, "description"), "\"game_functions\" its functions must have \"description\" as the first field");
		grug_assert(field->value->type == JSON_NODE_STRING, "\"game_functions\" its function descriptions must be strings");
		char *description = push_mod_api_string(field->value->string);
		grug_assert(!streq(description, ""), "\"game_functions\" its function descriptions must not be an empty string");

		bool seen_return_type = false;

		if (fn.field_count > 1) {
			field++;

			if (streq(field->key, "return_type")) {
				grug_assert(field->value->type == JSON_NODE_STRING, "\"game_functions\" its function return types must be strings");
				grug_fn.return_type = parse_type(field->value->string);
				grug_assert(grug_fn.return_type != type_resource, "\"game_functions\" its function return types must not be 'resource'");
				grug_assert(grug_fn.return_type != type_entity, "\"game_functions\" its function return types must not be 'entity'");
				seen_return_type = true;
				field++;
			} else {
				grug_assert(streq(field->key, "arguments"), "\"game_functions\" its second field was something other than \"return_type\" and \"arguments\"");
			}
		} else {
			grug_fn.return_type = type_void;
		}

		if ((!seen_return_type && fn.field_count > 1) || fn.field_count > 2) {
			grug_assert(streq(field->key, "arguments"), "\"game_functions\" its second or third field was something other than \"arguments\"");

			grug_assert(field->value->type == JSON_NODE_ARRAY, "\"game_functions\" its function arguments must be arrays");
			struct json_node *value = field->value->array.values;

			grug_fn.arguments = grug_arguments + grug_arguments_size;
			grug_fn.argument_count = field->value->array.value_count;
			grug_assert(grug_fn.argument_count > 0, "\"game_functions\" its \"arguments\" array must not be empty (just remove the \"arguments\" key entirely)");

			for (size_t argument_index = 0; argument_index < grug_fn.argument_count; argument_index++) {
				struct argument grug_arg = {0};

				grug_assert(value->type == JSON_NODE_OBJECT, "\"game_functions\" its function arguments must only contain objects");
				grug_assert(value->object.field_count >= 2, "\"game_functions\" must have the function argument fields \"name\" and \"type\"");
				grug_assert(value->object.field_count <= 3, "\"game_functions\" its function arguments can't have more than 3 fields");
				struct json_field *argument_field = value->object.fields;

				grug_assert(streq(argument_field->key, "name"), "\"game_functions\" its function arguments must always have \"name\" as their first field");
				grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"game_functions\" its function arguments must always have string values");
				grug_arg.name = push_mod_api_string(argument_field->value->string);
				argument_field++;

				grug_assert(streq(argument_field->key, "type"), "\"game_functions\" its function arguments must always have \"type\" as their second field");
				grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"game_functions\" its function arguments must always have string values");
				grug_arg.type = parse_type(argument_field->value->string);
				argument_field++;

				if (grug_arg.type == type_resource) {
					grug_assert(value->object.field_count == 3 && streq(argument_field->key, "resource_extension"), "\"game_functions\" its function arguments has a \"type\" field with the value \"resource\", which means a \"resource_extension\" field is required");
					grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"game_functions\" its function argument fields must always have string values");
					grug_arg.resource_extension = push_mod_api_string(argument_field->value->string);
				} else if (grug_arg.type == type_entity) {
					grug_assert(value->object.field_count == 3 && streq(argument_field->key, "entity_type"), "\"game_functions\" its function arguments has a \"type\" field with the value \"entity\", which means an \"entity_type\" field is required");
					grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"game_functions\" its function argument fields must always have string values");
					grug_arg.entity_type = push_mod_api_string(argument_field->value->string);
				} else {
					grug_assert(value->object.field_count == 2, "\"game_functions\" its function argument fields had an unexpected 3rd \"%s\" field", argument_field->key);
				}

				push_grug_argument(grug_arg);
				value++;
			}
		}

		push_grug_game_function(grug_fn);
	}

	hash_game_fns();
}

static void init_on_fns(struct json_object fns) {
	for (size_t fn_index = 0; fn_index < fns.field_count; fn_index++) {
		struct grug_on_function grug_fn = {0};

		grug_fn.name = push_mod_api_string(fns.fields[fn_index].key);
		grug_assert(!streq(grug_fn.name, ""), "\"on_functions\" its function names must not be an empty string");
		grug_assert(starts_with(grug_fn.name, "on_"), "\"on_functions\" its function names must start with 'on_'");

		grug_assert(fns.fields[fn_index].value->type == JSON_NODE_OBJECT, "\"on_functions\" its array must only contain objects");
		struct json_object fn = fns.fields[fn_index].value->object;
		grug_assert(fn.field_count >= 1, "\"on_functions\" its objects must have at least a \"description\" field");
		grug_assert(fn.field_count <= 2, "\"on_functions\" its objects must not have more than 2 fields");

		struct json_field *field = fn.fields;

		grug_assert(streq(field->key, "description"), "\"on_functions\" its functions must have \"description\" as the first field");
		grug_assert(field->value->type == JSON_NODE_STRING, "\"on_functions\" its function descriptions must be strings");
		char *description = push_mod_api_string(field->value->string);
		grug_assert(!streq(description, ""), "\"on_functions\" its function descriptions must not be an empty string");

		if (fn.field_count > 1) {
			field++;

			grug_assert(streq(field->key, "arguments"), "\"on_functions\" its functions must have \"arguments\" as the second field");
			grug_assert(field->value->type == JSON_NODE_ARRAY, "\"on_functions\" its function arguments must be arrays");
			struct json_node *value = field->value->array.values;

			grug_fn.arguments = grug_arguments + grug_arguments_size;
			grug_fn.argument_count = field->value->array.value_count;

			for (size_t argument_index = 0; argument_index < grug_fn.argument_count; argument_index++) {
				struct argument grug_arg = {0};

				grug_assert(value->type == JSON_NODE_OBJECT, "\"on_functions\" its function arguments must only contain objects");
				grug_assert(value->object.field_count == 2, "\"on_functions\" its function arguments must only contain a name and type field");
				struct json_field *argument_field = value->object.fields;

				grug_assert(streq(argument_field->key, "name"), "\"on_functions\" its function arguments must always have \"name\" as their first field");
				grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"on_functions\" its function arguments must always have string values");
				grug_arg.name = push_mod_api_string(argument_field->value->string);
				argument_field++;

				grug_assert(streq(argument_field->key, "type"), "\"on_functions\" its function arguments must always have \"type\" as their second field");
				grug_assert(argument_field->value->type == JSON_NODE_STRING, "\"on_functions\" its function arguments must always have string values");
				grug_arg.type = parse_type(argument_field->value->string);
				grug_assert(grug_arg.type != type_resource, "\"on_functions\" its function argument types must not be 'resource'");
				grug_assert(grug_arg.type != type_entity, "\"on_functions\" its function argument types must not be 'entity'");
				argument_field++;

				push_grug_argument(grug_arg);
				value++;
			}
		}

		push_grug_on_function(grug_fn);
	}
}

static void init_entities(struct json_object entities) {
	for (size_t entity_field_index = 0; entity_field_index < entities.field_count; entity_field_index++) {
		struct grug_entity entity = {0};

		entity.name = push_mod_api_string(entities.fields[entity_field_index].key);
		grug_assert(!streq(entity.name, ""), "\"entities\" its names must not be an empty string");

		grug_assert(entities.fields[entity_field_index].value->type == JSON_NODE_OBJECT, "\"entities\" must only contain object values");
		struct json_object fn = entities.fields[entity_field_index].value->object;
		grug_assert(fn.field_count >= 1, "\"entities\" its objects must have at least a \"description\" field");
		grug_assert(fn.field_count <= 3, "\"entities\" its objects must not have more than 3 fields");

		struct json_field *field = fn.fields;

		grug_assert(streq(field->key, "description"), "\"entities\" must have \"description\" as the first field");
		grug_assert(field->value->type == JSON_NODE_STRING, "\"entities\" its descriptions must be strings");
		char *description = push_mod_api_string(field->value->string);
		grug_assert(!streq(description, ""), "\"entities\" its descriptions must not be an empty string");

		bool seen_fields = false;

		if (fn.field_count > 1) {
			field++;

			if (streq(field->key, "fields")) {
				grug_assert(field->value->type == JSON_NODE_ARRAY, "\"entities\" its \"fields\" must be arrays");
				struct json_node *value = field->value->array.values;
				entity.fields = grug_arguments + grug_arguments_size;
				entity.field_count = field->value->array.value_count;

				for (size_t field_index = 0; field_index < entity.field_count; field_index++) {
					struct argument grug_field = {0};

					grug_assert(value->type == JSON_NODE_OBJECT, "\"entities\" its fields must only contain objects");
					grug_assert(value->object.field_count >= 2, "\"entities\" must have the fields \"name\" and \"type\"");
					grug_assert(value->object.field_count <= 3, "\"entities\" can't have more than 3 fields");
					struct json_field *json_field = value->object.fields;

					grug_assert(streq(json_field->key, "name"), "\"entities\" its fields must always have \"name\" as their first field");
					grug_assert(json_field->value->type == JSON_NODE_STRING, "\"entities\" its fields must always have string values");
					grug_field.name = push_mod_api_string(json_field->value->string);
					json_field++;

					grug_assert(streq(json_field->key, "type"), "\"entities\" its fields must always have \"type\" as their second field");
					grug_assert(json_field->value->type == JSON_NODE_STRING, "\"entities\" its fields must always have string values");
					grug_field.type = parse_type(json_field->value->string);
					json_field++;

					grug_assert(grug_field.type != type_id, "\"entities\" has a \"type\" field with the value \"id\", which isn't allowed");

					if (grug_field.type == type_resource) {
						grug_assert(value->object.field_count == 3 && streq(json_field->key, "resource_extension"), "\"entities\" has a \"type\" field with the value \"resource\", which means a \"resource_extension\" field is required");
						grug_assert(json_field->value->type == JSON_NODE_STRING, "\"entities\" its fields must always have string values");
						grug_field.resource_extension = push_mod_api_string(json_field->value->string);
					} else if (grug_field.type == type_entity) {
						grug_assert(value->object.field_count == 3 && streq(json_field->key, "entity_type"), "\"entities\" has a \"type\" field with the value \"entity\", which means an \"entity_type\" field is required");
						grug_assert(json_field->value->type == JSON_NODE_STRING, "\"entities\" its fields must always have string values");
						grug_field.entity_type = push_mod_api_string(json_field->value->string);
					} else {
						grug_assert(value->object.field_count == 2, "\"entities\" had an unexpected 3rd \"%s\" field", json_field->key);
					}

					push_grug_argument(grug_field);
					value++;
				}

				seen_fields = true;
				field++;
			} else {
				grug_assert(streq(field->key, "on_functions"), "\"entities\" its second field was something other than \"fields\" and \"on_functions\"");
			}
		}

		if ((!seen_fields && fn.field_count > 1) || fn.field_count > 2) {
			grug_assert(streq(field->key, "on_functions"), "\"entities\" its second or third field was something other than \"on_functions\"");
			grug_assert(field->value->type == JSON_NODE_OBJECT, "\"entities\" its \"on_functions\" field must have an object as its value");
			entity.on_functions = grug_on_functions + grug_on_functions_size;
			entity.on_function_count = field->value->object.field_count;
			init_on_fns(field->value->object);
		}

		push_grug_entity(entity);
	}
}

static void parse_mod_api_json(char *mod_api_json_path) {
	struct json_node node;
	json(mod_api_json_path, &node);

	grug_assert(node.type == JSON_NODE_OBJECT, "mod_api.json its root must be an object");
	struct json_object root_object = node.object;

	grug_assert(root_object.field_count == 2, "mod_api.json must have these 2 fields, in this order: \"entities\", \"game_functions\"");

	struct json_field *field = root_object.fields;

	grug_assert(streq(field->key, "entities"), "mod_api.json its root object must have \"entities\" as its first field");
	grug_assert(field->value->type == JSON_NODE_OBJECT, "mod_api.json its \"entities\" field must have an object as its value");
	init_entities(field->value->object);
	field++;

	grug_assert(streq(field->key, "game_functions"), "mod_api.json its root object must have \"game_functions\" as its third field");
	grug_assert(field->value->type == JSON_NODE_OBJECT, "mod_api.json its \"game_functions\" field must have an object as its value");
	init_game_fns(field->value->object);
}
