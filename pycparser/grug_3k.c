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

//// READING

#define MAX_CHARACTERS 420420

static char grug_text[MAX_CHARACTERS];

static void read_file(char *path) {
	FILE *f = fopen(path, "rb");
	grug_assert(f, "fopen: %s", strerror(errno));

	grug_assert(fseek(f, 0, SEEK_END) == 0, "fseek: %s", strerror(errno));

	long count = ftell(f);
	grug_assert(count != -1, "ftell: %s", strerror(errno));
	grug_assert(count < MAX_CHARACTERS, "There are more than %d characters in the grug file, exceeding MAX_CHARACTERS", MAX_CHARACTERS);

	rewind(f);

	size_t bytes_read = fread(grug_text, sizeof(char), count, f);
	grug_assert(bytes_read == (size_t)count || feof(f), "fread error");

	grug_text[count] = '\0';

	grug_assert(fclose(f) == 0, "fclose: %s", strerror(errno));
}

//// TOKENIZATION

#define MAX_TOKENS 420420
#define MAX_TOKEN_STRINGS_CHARACTERS 420420
#define SPACES_PER_INDENT 4

enum token_type {
	OPEN_PARENTHESIS_TOKEN,
	CLOSE_PARENTHESIS_TOKEN,
	OPEN_BRACE_TOKEN,
	CLOSE_BRACE_TOKEN,
	PLUS_TOKEN,
	MINUS_TOKEN,
	MULTIPLICATION_TOKEN,
	DIVISION_TOKEN,
	REMAINDER_TOKEN,
	COMMA_TOKEN,
	COLON_TOKEN,
	PERIOD_TOKEN,
	NEWLINE_TOKEN,
	EQUALS_TOKEN,
	NOT_EQUALS_TOKEN,
	ASSIGNMENT_TOKEN,
	GREATER_OR_EQUAL_TOKEN,
	GREATER_TOKEN,
	LESS_OR_EQUAL_TOKEN,
	LESS_TOKEN,
	AND_TOKEN,
	OR_TOKEN,
	NOT_TOKEN,
	TRUE_TOKEN,
	FALSE_TOKEN,
	IF_TOKEN,
	ELSE_TOKEN,
	WHILE_TOKEN,
	BREAK_TOKEN,
	RETURN_TOKEN,
	CONTINUE_TOKEN,
	SPACE_TOKEN,
	INDENTATION_TOKEN,
	STRING_TOKEN,
	WORD_TOKEN,
	I32_TOKEN,
	F32_TOKEN,
	COMMENT_TOKEN,
};

struct token {
	enum token_type type;
	char *str;
};
static char *get_token_type_str[] = {
	[OPEN_PARENTHESIS_TOKEN] = "OPEN_PARENTHESIS_TOKEN",
	[CLOSE_PARENTHESIS_TOKEN] = "CLOSE_PARENTHESIS_TOKEN",
	[OPEN_BRACE_TOKEN] = "OPEN_BRACE_TOKEN",
	[CLOSE_BRACE_TOKEN] = "CLOSE_BRACE_TOKEN",
	[PLUS_TOKEN] = "PLUS_TOKEN",
	[MINUS_TOKEN] = "MINUS_TOKEN",
	[MULTIPLICATION_TOKEN] = "MULTIPLICATION_TOKEN",
	[DIVISION_TOKEN] = "DIVISION_TOKEN",
	[REMAINDER_TOKEN] = "REMAINDER_TOKEN",
	[COMMA_TOKEN] = "COMMA_TOKEN",
	[COLON_TOKEN] = "COLON_TOKEN",
	[PERIOD_TOKEN] = "PERIOD_TOKEN",
	[NEWLINE_TOKEN] = "NEWLINE_TOKEN",
	[EQUALS_TOKEN] = "EQUALS_TOKEN",
	[NOT_EQUALS_TOKEN] = "NOT_EQUALS_TOKEN",
	[ASSIGNMENT_TOKEN] = "ASSIGNMENT_TOKEN",
	[GREATER_OR_EQUAL_TOKEN] = "GREATER_OR_EQUAL_TOKEN",
	[GREATER_TOKEN] = "GREATER_TOKEN",
	[LESS_OR_EQUAL_TOKEN] = "LESS_OR_EQUAL_TOKEN",
	[LESS_TOKEN] = "LESS_TOKEN",
	[AND_TOKEN] = "AND_TOKEN",
	[OR_TOKEN] = "OR_TOKEN",
	[NOT_TOKEN] = "NOT_TOKEN",
	[TRUE_TOKEN] = "TRUE_TOKEN",
	[FALSE_TOKEN] = "FALSE_TOKEN",
	[IF_TOKEN] = "IF_TOKEN",
	[ELSE_TOKEN] = "ELSE_TOKEN",
	[WHILE_TOKEN] = "WHILE_TOKEN",
	[BREAK_TOKEN] = "BREAK_TOKEN",
	[RETURN_TOKEN] = "RETURN_TOKEN",
	[CONTINUE_TOKEN] = "CONTINUE_TOKEN",
	[SPACE_TOKEN] = "SPACE_TOKEN",
	[INDENTATION_TOKEN] = "INDENTATION_TOKEN",
	[STRING_TOKEN] = "STRING_TOKEN",
	[WORD_TOKEN] = "WORD_TOKEN",
	[I32_TOKEN] = "I32_TOKEN",
	[F32_TOKEN] = "F32_TOKEN",
	[COMMENT_TOKEN] = "COMMENT_TOKEN",
};
static struct token tokens[MAX_TOKENS];
static size_t tokens_size;

static char token_strings[MAX_TOKEN_STRINGS_CHARACTERS];
static size_t token_strings_size;

static void reset_tokenization(void) {
	tokens_size = 0;
	token_strings_size = 0;
}

static size_t max_size_t(size_t a, size_t b) {
	if (a > b) {
		return a;
	}
	return b;
}

static struct token peek_token(size_t token_index) {
	grug_assert(token_index < tokens_size, "token_index %zu was out of bounds in peek_token()", token_index);
	return tokens[token_index];
}

static struct token consume_token(size_t *token_index_ptr) {
	return peek_token((*token_index_ptr)++);
}

static void print_tokens(void) {
	size_t longest_token_type_len = 0;
	for (size_t i = 0; i < tokens_size; i++) {
		struct token token = peek_token(i);
		char *token_type_str = get_token_type_str[token.type];
		longest_token_type_len = max_size_t(strlen(token_type_str), longest_token_type_len);
	}

	// Leave enough space for the word "index", but if the index exceeds 99999, add extra spaces
	// In pseudocode this does longest_index = max(floor(log10(tokens.size)), strlen("index"))
	size_t longest_index = 1;
	size_t n = tokens_size;
	while (true) {
		n /= 10;
		if (n == 0) {
			break;
		}
		longest_index++;
	}
	longest_index = max_size_t(longest_index, strlen("index"));

	grug_log("| %-*s | %-*s | str\n", (int)longest_index, "index", (int)longest_token_type_len, "type");

	for (size_t i = 0; i < tokens_size; i++) {
		struct token token = peek_token(i);

		grug_log("| %*zu ", (int)longest_index, i);

		char *token_type_str = get_token_type_str[token.type];
		grug_log("| %*s ", (int)longest_token_type_len, token_type_str);

		grug_log("| '%s'\n", token.type == NEWLINE_TOKEN ? "\\n" : token.str);
	}
}

// Here are some examples, where the part in <> indicates the character_index character
// "" => 1
// "<a>" => 1
// "a<b>" => 1
// "<\n>" => 1
// "\n<a>" => 2
// "\n<\n>" => 2
static size_t get_character_line_number(size_t character_index) {
	size_t line_number = 1;

	for (size_t i = 0; i < character_index; i++) {
		if (grug_text[i] == '\n' || (grug_text[i] == '\r' && grug_text[i + 1] == '\n')) {
			line_number++;
		}
	}

	return line_number;
}

static char *get_escaped_char(char *str) {
	switch (*str) {
	case '\f':
		return "\\f";
	case '\n':
		return "\\n";
	case '\r':
		return "\\r";
	case '\t':
		return "\\t";
	case '\v':
		return "\\v";
	}
	return str;
}

static bool is_escaped_char(char c) {
	return isspace(c) && c != ' ';
}

static bool is_end_of_word(char c) {
	return !isalnum(c) && c != '_';
}

static char *push_token_string(char *slice_start, size_t length) {
	grug_assert(token_strings_size + length < MAX_TOKEN_STRINGS_CHARACTERS, "There are more than %d characters in the token_strings array, exceeding MAX_TOKEN_STRINGS_CHARACTERS", MAX_TOKEN_STRINGS_CHARACTERS);

	char *new_str = token_strings + token_strings_size;

	for (size_t i = 0; i < length; i++) {
		token_strings[token_strings_size++] = slice_start[i];
	}
	token_strings[token_strings_size++] = '\0';

	return new_str;
}

static void push_token(enum token_type type, char *str, size_t len) {
	grug_assert(tokens_size < MAX_TOKENS, "There are more than %d tokens in the grug file, exceeding MAX_TOKENS", MAX_TOKENS);
	tokens[tokens_size++] = (struct token){
		.type = type,
		.str = push_token_string(str, len),
	};
}

static void tokenize(void) {
	reset_tokenization();

	size_t i = 0;
	while (grug_text[i]) {
		if (       grug_text[i] == '(') {
			push_token(OPEN_PARENTHESIS_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == ')') {
			push_token(CLOSE_PARENTHESIS_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '{') {
			push_token(OPEN_BRACE_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '}') {
			push_token(CLOSE_BRACE_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '+') {
			push_token(PLUS_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '-') {
			push_token(MINUS_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '*') {
			push_token(MULTIPLICATION_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '/') {
			push_token(DIVISION_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '%') {
			push_token(REMAINDER_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == ',') {
			push_token(COMMA_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == ':') {
			push_token(COLON_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '.') {
			push_token(PERIOD_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '\n') {
			push_token(NEWLINE_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '=' && grug_text[i + 1] == '=') {
			push_token(EQUALS_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i] == '!' && grug_text[i + 1] == '=') {
			push_token(NOT_EQUALS_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i] == '=') {
			push_token(ASSIGNMENT_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '>' && grug_text[i + 1] == '=') {
			push_token(GREATER_OR_EQUAL_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i] == '>') {
			push_token(GREATER_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i] == '<' && grug_text[i + 1] == '=') {
			push_token(LESS_OR_EQUAL_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i] == '<') {
			push_token(LESS_TOKEN, grug_text+i, 1);
			i += 1;
		} else if (grug_text[i + 0] == 'a' && grug_text[i + 1] == 'n' && grug_text[i + 2] == 'd' && is_end_of_word(grug_text[i + 3])) {
			push_token(AND_TOKEN, grug_text+i, 3);
			i += 3;
		} else if (grug_text[i + 0] == 'o' && grug_text[i + 1] == 'r' && is_end_of_word(grug_text[i + 2])) {
			push_token(OR_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i + 0] == 'n' && grug_text[i + 1] == 'o' && grug_text[i + 2] == 't' && is_end_of_word(grug_text[i + 3])) {
			push_token(NOT_TOKEN, grug_text+i, 3);
			i += 3;
		} else if (grug_text[i + 0] == 't' && grug_text[i + 1] == 'r' && grug_text[i + 2] == 'u' && grug_text[i + 3] == 'e' && is_end_of_word(grug_text[i + 4])) {
			push_token(TRUE_TOKEN, grug_text+i, 4);
			i += 4;
		} else if (grug_text[i + 0] == 'f' && grug_text[i + 1] == 'a' && grug_text[i + 2] == 'l' && grug_text[i + 3] == 's' && grug_text[i + 4] == 'e' && is_end_of_word(grug_text[i + 5])) {
			push_token(FALSE_TOKEN, grug_text+i, 5);
			i += 5;
		} else if (grug_text[i + 0] == 'i' && grug_text[i + 1] == 'f' && is_end_of_word(grug_text[i + 2])) {
			push_token(IF_TOKEN, grug_text+i, 2);
			i += 2;
		} else if (grug_text[i + 0] == 'e' && grug_text[i + 1] == 'l' && grug_text[i + 2] == 's' && grug_text[i + 3] == 'e' && is_end_of_word(grug_text[i + 4])) {
			push_token(ELSE_TOKEN, grug_text+i, 4);
			i += 4;
		} else if (grug_text[i + 0] == 'w' && grug_text[i + 1] == 'h' && grug_text[i + 2] == 'i' && grug_text[i + 3] == 'l' && grug_text[i + 4] == 'e' && is_end_of_word(grug_text[i + 5])) {
			push_token(WHILE_TOKEN, grug_text+i, 5);
			i += 5;
		} else if (grug_text[i + 0] == 'b' && grug_text[i + 1] == 'r' && grug_text[i + 2] == 'e' && grug_text[i + 3] == 'a' && grug_text[i + 4] == 'k' && is_end_of_word(grug_text[i + 5])) {
			push_token(BREAK_TOKEN, grug_text+i, 5);
			i += 5;
		} else if (grug_text[i + 0] == 'r' && grug_text[i + 1] == 'e' && grug_text[i + 2] == 't' && grug_text[i + 3] == 'u' && grug_text[i + 4] == 'r' && grug_text[i + 5] == 'n' && is_end_of_word(grug_text[i + 6])) {
			push_token(RETURN_TOKEN, grug_text+i, 6);
			i += 6;
		} else if (grug_text[i + 0] == 'c' && grug_text[i + 1] == 'o' && grug_text[i + 2] == 'n' && grug_text[i + 3] == 't' && grug_text[i + 4] == 'i' && grug_text[i + 5] == 'n' && grug_text[i + 6] == 'u' && grug_text[i + 7] == 'e' && is_end_of_word(grug_text[i + 8])) {
			push_token(CONTINUE_TOKEN, grug_text+i, 8);
			i += 8;
		} else if (grug_text[i] == ' ') {
			if (grug_text[i + 1] != ' ') {
				push_token(SPACE_TOKEN, grug_text+i, 1);
				i += 1;
				continue;
			}

			char *str = grug_text+i;
			size_t old_i = i;

			do {
				i++;
			} while (grug_text[i] == ' ');

			size_t spaces = i - old_i;

			grug_assert(spaces % SPACES_PER_INDENT == 0, "Encountered %zu spaces, while indentation expects multiples of %d spaces, on line %zu of the grug text file", spaces, SPACES_PER_INDENT, get_character_line_number(i));

			push_token(INDENTATION_TOKEN, str, spaces);
		} else if (grug_text[i] == '\"') {
			char *str = grug_text+i + 1;
			size_t old_i = i + 1;

			size_t open_double_quote_index = i;

			do {
				i++;
				grug_assert(grug_text[i] != '\0', "Unclosed \" on line %zu of the grug text file", get_character_line_number(open_double_quote_index + 1));
			} while (grug_text[i] != '\"');
			i++;

			push_token(STRING_TOKEN, str, i - old_i - 1);
		} else if (isalpha(grug_text[i]) || grug_text[i] == '_') {
			char *str = grug_text+i;
			size_t old_i = i;

			do {
				i++;
			} while (isalnum(grug_text[i]) || grug_text[i] == '_');

			push_token(WORD_TOKEN, str, i - old_i);
		} else if (isdigit(grug_text[i])) {
			char *str = grug_text+i;
			size_t old_i = i;

			bool seen_period = false;

			i++;
			while (isdigit(grug_text[i]) || grug_text[i] == '.') {
				if (grug_text[i] == '.') {
					grug_assert(!seen_period, "Encountered two '.' periods in a number on line %zu of the grug text file", get_character_line_number(i));
					seen_period = true;
				}
				i++;
			}

			if (seen_period) {
				grug_assert(grug_text[i - 1] != '.', "Missing digit after decimal point in '%.*s'", (int)(i - old_i), str);
				push_token(F32_TOKEN, str, i - old_i);
			} else {
				push_token(I32_TOKEN, str, i - old_i);
			}
		} else if (grug_text[i] == '#') {
			i++;

			grug_assert(grug_text[i] == ' ', "Expected a single space after the '#' on line %zu of the grug text file", get_character_line_number(i));
			i++;

			char *str = grug_text+i;
			size_t old_i = i;

			while (true) {
				if (!isprint(grug_text[i])) {
					if (grug_text[i] == '\r' || grug_text[i] == '\n' || grug_text[i] == '\0') {
						break;
					}

					grug_error("Unexpected unprintable character '%.*s' on line %zu of the grug text file", is_escaped_char(grug_text[i]) ? 2 : 1, get_escaped_char(&grug_text[i]), get_character_line_number(i + 1));
				}
				i++;
			}

			size_t len = i - old_i;

			grug_assert(len > 0, "Expected the comment to contain some text, on line %zu of the grug text file", get_character_line_number(i));

			grug_assert(!isspace(grug_text[i - 1]), "A comment has trailing whitespace on line %zu of the grug text file", get_character_line_number(i));

			push_token(COMMENT_TOKEN, str, len);
		} else {
			grug_error("Unrecognized character '%.*s' on line %zu of the grug text file", is_escaped_char(grug_text[i]) ? 2 : 1, get_escaped_char(&grug_text[i]), get_character_line_number(i + 1));
		}
	}
}

//// PARSING

#define MAX_EXPRS 420420
#define MAX_FIELDS 420420
#define MAX_STATEMENTS 420420
#define MAX_GLOBAL_STATEMENTS 420420
#define MAX_ARGUMENTS 420420
#define MAX_ON_FNS 420420
#define MAX_HELPER_FNS 420420
#define MAX_GLOBAL_VARIABLES 420420
#define MAX_CALLED_HELPER_FN_NAMES 420420
#define MAX_CALL_ARGUMENTS_PER_STACK_FRAME 69
#define MAX_STATEMENTS_PER_SCOPE 1337

struct literal_expr {
	union {
		char *string;
		i32 i32;
		struct {
			f32 value;
			char *string;
		} f32;
	};
};

struct unary_expr {
	enum token_type operator;
	struct expr *expr;
};

struct binary_expr {
	struct expr *left_expr;
	enum token_type operator;
	struct expr *right_expr;
};

struct call_expr {
	char *fn_name;
	struct expr *arguments;
	size_t argument_count;
};

enum expr_type {
	TRUE_EXPR,
	FALSE_EXPR,
	STRING_EXPR,
	RESOURCE_EXPR,
	ENTITY_EXPR,
	IDENTIFIER_EXPR,
	I32_EXPR,
	F32_EXPR,
	UNARY_EXPR,
	BINARY_EXPR,
	LOGICAL_EXPR,
	CALL_EXPR,
	PARENTHESIZED_EXPR,
};
struct expr {
	enum expr_type type;
	enum type result_type;
	union {
		struct literal_expr literal;
		struct unary_expr unary;
		struct binary_expr binary;
		struct call_expr call;
		struct expr *parenthesized;
	};
};
static char *get_expr_type_str[] = {
	[TRUE_EXPR] = "TRUE_EXPR",
	[FALSE_EXPR] = "FALSE_EXPR",
	[STRING_EXPR] = "STRING_EXPR",
	[RESOURCE_EXPR] = "RESOURCE_EXPR",
	[ENTITY_EXPR] = "ENTITY_EXPR",
	[IDENTIFIER_EXPR] = "IDENTIFIER_EXPR",
	[I32_EXPR] = "I32_EXPR",
	[F32_EXPR] = "F32_EXPR",
	[UNARY_EXPR] = "UNARY_EXPR",
	[BINARY_EXPR] = "BINARY_EXPR",
	[LOGICAL_EXPR] = "LOGICAL_EXPR",
	[CALL_EXPR] = "CALL_EXPR",
	[PARENTHESIZED_EXPR] = "PARENTHESIZED_EXPR",
};
static struct expr exprs[MAX_EXPRS];
static size_t exprs_size;

struct field {
	char *key;
	struct expr expr_value;
};
static struct field fields[MAX_FIELDS];
static size_t fields_size;

struct compound_literal {
	struct field *fields;
	size_t field_count;
};

struct variable_statement {
	char *name;
	enum type type;
	bool has_type;
	struct expr *assignment_expr;
};

struct call_statement {
	struct expr *expr;
};

struct if_statement {
	struct expr condition;
	struct statement *if_body_statements;
	size_t if_body_statement_count;
	struct statement *else_body_statements;
	size_t else_body_statement_count;
};

struct return_statement {
	struct expr *value;
	bool has_value;
};

struct while_statement {
	struct expr condition;
	struct statement *body_statements;
	size_t body_statement_count;
};

enum statement_type {
	VARIABLE_STATEMENT,
	CALL_STATEMENT,
	IF_STATEMENT,
	RETURN_STATEMENT,
	WHILE_STATEMENT,
	BREAK_STATEMENT,
	CONTINUE_STATEMENT,
	EMPTY_LINE_STATEMENT,
	COMMENT_STATEMENT,
};
struct statement {
	enum statement_type type;
	union {
		struct variable_statement variable_statement;
		struct call_statement call_statement;
		struct if_statement if_statement;
		struct return_statement return_statement;
		struct while_statement while_statement;
		char *comment;
	};
};
static char *get_statement_type_str[] = {
	[VARIABLE_STATEMENT] = "VARIABLE_STATEMENT",
	[CALL_STATEMENT] = "CALL_STATEMENT",
	[IF_STATEMENT] = "IF_STATEMENT",
	[RETURN_STATEMENT] = "RETURN_STATEMENT",
	[WHILE_STATEMENT] = "WHILE_STATEMENT",
	[BREAK_STATEMENT] = "BREAK_STATEMENT",
	[CONTINUE_STATEMENT] = "CONTINUE_STATEMENT",
	[EMPTY_LINE_STATEMENT] = "EMPTY_LINE_STATEMENT",
	[COMMENT_STATEMENT] = "COMMENT_STATEMENT",
};
static struct statement statements[MAX_STATEMENTS];
static size_t statements_size;

enum global_statement_type {
	GLOBAL_DEFINE_FN,
	GLOBAL_VARIABLE,
	GLOBAL_ON_FN,
	GLOBAL_HELPER_FN,
	GLOBAL_EMPTY_LINE,
	GLOBAL_COMMENT,
};
struct global_statement {
	enum global_statement_type type;
	union {
		struct global_variable_statement *global_variable;
		struct on_fn *on_fn;
		struct helper_fn *helper_fn;
		char *comment;
	};
};
static char *get_global_statement_type_str[] = {
	[GLOBAL_DEFINE_FN] = "GLOBAL_DEFINE_FN",
	[GLOBAL_VARIABLE] = "GLOBAL_VARIABLE",
	[GLOBAL_ON_FN] = "GLOBAL_ON_FN",
	[GLOBAL_HELPER_FN] = "GLOBAL_HELPER_FN",
	[GLOBAL_EMPTY_LINE] = "GLOBAL_EMPTY_LINE",
	[GLOBAL_COMMENT] = "GLOBAL_COMMENT",
};
static struct global_statement global_statements[MAX_GLOBAL_STATEMENTS];
static size_t global_statements_size;

static struct argument arguments[MAX_ARGUMENTS];
static size_t arguments_size;

struct parsed_define_fn {
	char *return_type;
	struct compound_literal returned_compound_literal;
};
static struct parsed_define_fn define_fn;

struct on_fn {
	char *fn_name;
	struct argument *arguments;
	size_t argument_count;
	struct statement *body_statements;
	size_t body_statement_count;
	bool calls_helper_fn;
	bool contains_while_loop;
	bool contains_i32_operation;
};
static struct on_fn on_fns[MAX_ON_FNS];
static size_t on_fns_size;

struct helper_fn {
	char *fn_name;
	struct argument *arguments;
	size_t argument_count;
	enum type return_type;
	struct statement *body_statements;
	size_t body_statement_count;
};
static struct helper_fn helper_fns[MAX_HELPER_FNS];
static size_t helper_fns_size;
static u32 buckets_helper_fns[MAX_HELPER_FNS];
static u32 chains_helper_fns[MAX_HELPER_FNS];

struct global_variable_statement {
	char *name;
	enum type type;
	struct expr assignment_expr;
};
static struct global_variable_statement global_variable_statements[MAX_GLOBAL_VARIABLES];
static size_t global_variable_statements_size;

static size_t indentation;

static char *called_helper_fn_names[MAX_CALLED_HELPER_FN_NAMES];
static size_t called_helper_fn_names_size;

static u32 buckets_called_helper_fn_names[MAX_CALLED_HELPER_FN_NAMES];
static u32 chains_called_helper_fn_names[MAX_CALLED_HELPER_FN_NAMES];

static void reset_parsing(void) {
	exprs_size = 0;
	fields_size = 0;
	statements_size = 0;
	global_statements_size = 0;
	arguments_size = 0;
	on_fns_size = 0;
	helper_fns_size = 0;
	global_variable_statements_size = 0;
	called_helper_fn_names_size = 0;
	memset(buckets_called_helper_fn_names, 0xff, sizeof(buckets_called_helper_fn_names));
}

static struct helper_fn *get_helper_fn(char *name) {
	if (helper_fns_size == 0) {
		return NULL;
	}

	u32 i = buckets_helper_fns[elf_hash(name) % helper_fns_size];

	while (true) {
		if (i == UINT32_MAX) {
			return NULL;
		}

		if (streq(name, helper_fns[i].fn_name)) {
			break;
		}

		i = chains_helper_fns[i];
	}

	return helper_fns + i;
}

static void hash_helper_fns(void) {
	memset(buckets_helper_fns, 0xff, helper_fns_size * sizeof(u32));

	for (size_t i = 0; i < helper_fns_size; i++) {
		char *name = helper_fns[i].fn_name;

		grug_assert(!get_helper_fn(name), "The function '%s' was defined several times in the same file", name);

		u32 bucket_index = elf_hash(name) % helper_fns_size;

		chains_helper_fns[i] = buckets_helper_fns[bucket_index];

		buckets_helper_fns[bucket_index] = i;
	}
}

static struct helper_fn *push_helper_fn(struct helper_fn helper_fn) {
	grug_assert(helper_fns_size < MAX_HELPER_FNS, "There are more than %d helper_fns in the grug file, exceeding MAX_HELPER_FNS", MAX_HELPER_FNS);
	helper_fns[helper_fns_size] = helper_fn;
	return helper_fns + helper_fns_size++;
}

static struct on_fn *push_on_fn(struct on_fn on_fn) {
	grug_assert(on_fns_size < MAX_ON_FNS, "There are more than %d on_fns in the grug file, exceeding MAX_ON_FNS", MAX_ON_FNS);
	on_fns[on_fns_size] = on_fn;
	return on_fns + on_fns_size++;
}

static struct statement *push_statement(struct statement statement) {
	grug_assert(statements_size < MAX_STATEMENTS, "There are more than %d statements in the grug file, exceeding MAX_STATEMENTS", MAX_STATEMENTS);
	statements[statements_size] = statement;
	return statements + statements_size++;
}

static struct expr *push_expr(struct expr expr) {
	grug_assert(exprs_size < MAX_EXPRS, "There are more than %d exprs in the grug file, exceeding MAX_EXPRS", MAX_EXPRS);
	exprs[exprs_size] = expr;
	return exprs + exprs_size++;
}

// Here are some examples, where the part in <> indicates the token_index token
// "" => 1
// "<a>" => 1
// "a<b>" => 1
// "<\n>" => 1
// "\n<a>" => 2
// "\n<\n>" => 2
static size_t get_token_line_number(size_t token_index) {
	assert(token_index < tokens_size);

	size_t line_number = 1;

	for (size_t i = 0; i < token_index; i++) {
		if (tokens[i].type == NEWLINE_TOKEN) {
			line_number++;
		}
	}

	return line_number;
}

static void assert_token_type(size_t token_index, unsigned int expected_type) {
	struct token token = peek_token(token_index);
	grug_assert(token.type == expected_type, "Expected token type %s, but got %s on line %zu", get_token_type_str[expected_type], get_token_type_str[token.type], get_token_line_number(token_index));
}

static void consume_token_type(size_t *token_index_ptr, unsigned int expected_type) {
	assert_token_type((*token_index_ptr)++, expected_type);
}

static void consume_newline(size_t *token_index_ptr) {
	assert_token_type(*token_index_ptr, NEWLINE_TOKEN);
	(*token_index_ptr)++;
}

static void consume_space(size_t *token_index_ptr) {
	assert_token_type(*token_index_ptr, SPACE_TOKEN);
	(*token_index_ptr)++;
}

static void consume_indentation(size_t *token_index_ptr) {
	assert_token_type(*token_index_ptr, INDENTATION_TOKEN);

	size_t spaces = strlen(peek_token(*token_index_ptr).str);

	grug_assert(spaces == indentation * SPACES_PER_INDENT, "Expected %zu spaces, but got %zu spaces on line %zu", indentation * SPACES_PER_INDENT, spaces, get_token_line_number(*token_index_ptr));

	(*token_index_ptr)++;
}

static bool is_end_of_block(size_t *token_index_ptr) {
	assert(indentation > 0);

	struct token token = peek_token(*token_index_ptr);
	if (token.type == CLOSE_BRACE_TOKEN) {
		return true;
	} else if (token.type == NEWLINE_TOKEN) {
		return false;
	}

	grug_assert(token.type == INDENTATION_TOKEN, "Expected indentation, or an empty line, or '}', but got '%s' on line %zu", token.str, get_token_line_number(*token_index_ptr));

	size_t spaces = strlen(token.str);
	return spaces == (indentation - 1) * SPACES_PER_INDENT;
}

static f32 str_to_f32(char *str) {
	char *end;
	errno = 0;
	float f = strtof(str, &end);

	if (errno == ERANGE) {
		if (f == HUGE_VALF) {
			grug_error("The float '%s' is too big to fit in an f32", str);
		} else if (f == 0) {
			grug_error("The float '%s' is too small to fit in an f32", str);
		}
		// No need to check `f == -HUGE_VALF`, since the token can't ever start with a minus sign
		grug_unreachable();
	}

	// An f32 token only gets created when it starts with a digit,
	// so strtof() should at the very least have incremented `end` by 1
	assert(str != end);

	// This function can't ever have trailing characters,
	// since the number was tokenized
	assert(*end == '\0');

	return f;
}

// Inspiration: https://stackoverflow.com/a/12923949/13279557
static i32 str_to_i32(char *str) {
	char *end;
	errno = 0;
	long n = strtol(str, &end, 10);

	grug_assert(n <= INT32_MAX && !(errno == ERANGE && n == LONG_MAX), "The number %s is too big for an i32, which has a maximum value of %d", str, INT32_MAX);

	// This function can't ever return a negative number,
	// since the minus symbol gets tokenized separately
	assert(errno != ERANGE);
	assert(n >= 0);

	// This function can't ever have trailing characters,
	// since the number was tokenized
	assert(*end == '\0');

	return n;
}

static struct expr parse_expression(size_t *i);

static struct expr parse_primary(size_t *i) {
	struct token token = peek_token(*i);

	struct expr expr = {0};

	switch (token.type) {
		case OPEN_PARENTHESIS_TOKEN:
			(*i)++;
			expr.type = PARENTHESIZED_EXPR;
			expr.parenthesized = push_expr(parse_expression(i));
			consume_token_type(i, CLOSE_PARENTHESIS_TOKEN);
			return expr;
		case TRUE_TOKEN:
			(*i)++;
			expr.type = TRUE_EXPR;
			return expr;
		case FALSE_TOKEN:
			(*i)++;
			expr.type = FALSE_EXPR;
			return expr;
		case STRING_TOKEN:
			(*i)++;
			expr.type = STRING_EXPR;
			expr.literal.string = token.str;
			return expr;
		case WORD_TOKEN:
			(*i)++;
			expr.type = IDENTIFIER_EXPR;
			expr.literal.string = token.str;
			return expr;
		case I32_TOKEN:
			(*i)++;
			expr.type = I32_EXPR;
			expr.literal.i32 = str_to_i32(token.str);
			return expr;
		case F32_TOKEN:
			(*i)++;
			expr.type = F32_EXPR;
			expr.literal.f32.value = str_to_f32(token.str);
			expr.literal.f32.string = token.str;
			return expr;
		default:
			grug_error("Expected a primary expression token, but got token type %s on line %zu", get_token_type_str[token.type], get_token_line_number(*i));
	}
}

static void push_called_helper_fn_name(char *name) {
	grug_assert(called_helper_fn_names_size < MAX_CALLED_HELPER_FN_NAMES, "There are more than %d called helper function names, exceeding MAX_CALLED_HELPER_FN_NAMES", MAX_CALLED_HELPER_FN_NAMES);

	called_helper_fn_names[called_helper_fn_names_size++] = name;
}

static bool seen_called_helper_fn_name(char *name) {
	if (called_helper_fn_names_size == 0) {
		return false;
	}

	u32 i = buckets_called_helper_fn_names[elf_hash(name) % MAX_CALLED_HELPER_FN_NAMES];

	while (true) {
		if (i == UINT32_MAX) {
			return false;
		}

		if (streq(name, called_helper_fn_names[i])) {
			break;
		}

		i = chains_called_helper_fn_names[i];
	}

	return true;
}

static void add_called_helper_fn_name(char *name) {
	if (!seen_called_helper_fn_name(name)) {
		u32 bucket_index = elf_hash(name) % MAX_CALLED_HELPER_FN_NAMES;

		chains_called_helper_fn_names[called_helper_fn_names_size] = buckets_called_helper_fn_names[bucket_index];

		buckets_called_helper_fn_names[bucket_index] = called_helper_fn_names_size;

		push_called_helper_fn_name(name);
	}
}

static struct expr parse_call(size_t *i) {
	struct expr expr = parse_primary(i);

	struct token token = peek_token(*i);
	if (token.type != OPEN_PARENTHESIS_TOKEN) {
		return expr;
	}

	(*i)++;

	grug_assert(expr.type == IDENTIFIER_EXPR, "Unexpected open parenthesis after non-identifier expression type %s on line %zu", get_expr_type_str[expr.type], get_token_line_number(*i - 2));
	expr.type = CALL_EXPR;

	expr.call.fn_name = expr.literal.string;

	if (starts_with(expr.call.fn_name, "helper_")) {
		add_called_helper_fn_name(expr.call.fn_name);
	}

	expr.call.argument_count = 0;

	token = peek_token(*i);
	if (token.type == CLOSE_PARENTHESIS_TOKEN) {
		(*i)++;
		return expr;
	}

	struct expr local_call_arguments[MAX_CALL_ARGUMENTS_PER_STACK_FRAME];

	while (true) {
		struct expr call_argument = parse_expression(i);

		grug_assert(expr.call.argument_count < MAX_CALL_ARGUMENTS_PER_STACK_FRAME, "There are more than %d arguments to a function call in one of the grug file's stack frames, exceeding MAX_CALL_ARGUMENTS_PER_STACK_FRAME", MAX_CALL_ARGUMENTS_PER_STACK_FRAME);
		local_call_arguments[expr.call.argument_count++] = call_argument;

		token = peek_token(*i);
		if (token.type != COMMA_TOKEN) {
			assert_token_type(*i, CLOSE_PARENTHESIS_TOKEN);
			(*i)++;
			break;
		}
		(*i)++;
		consume_space(i);
	}

	expr.call.arguments = exprs + exprs_size;
	for (size_t argument_index = 0; argument_index < expr.call.argument_count; argument_index++) {
		(void)push_expr(local_call_arguments[argument_index]);
	}

	return expr;
}

static struct expr parse_unary(size_t *i) {
	struct token token = peek_token(*i);
	if (token.type == MINUS_TOKEN
	 || token.type == NOT_TOKEN) {
		(*i)++;
		if (token.type == NOT_TOKEN) {
			consume_space(i);
		}

		struct expr expr = {0};

		expr.unary.operator = token.type;
		expr.unary.expr = push_expr(parse_unary(i));
		expr.type = UNARY_EXPR;

		return expr;
	}

	return parse_call(i);
}

static struct expr parse_factor(size_t *i) {
	struct expr expr = parse_unary(i);

	while (peek_token(*i).type == SPACE_TOKEN && (
		   peek_token(*i + 1).type == MULTIPLICATION_TOKEN
		|| peek_token(*i + 1).type == DIVISION_TOKEN
		|| peek_token(*i + 1).type == REMAINDER_TOKEN)) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_unary(i));
		expr.type = BINARY_EXPR;
	}

	return expr;
}

static struct expr parse_term(size_t *i) {
	struct expr expr = parse_factor(i);

	while (peek_token(*i).type == SPACE_TOKEN && (
		   peek_token(*i + 1).type == PLUS_TOKEN
		|| peek_token(*i + 1).type == MINUS_TOKEN)) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_factor(i));
		expr.type = BINARY_EXPR;
	}

	return expr;
}

static struct expr parse_comparison(size_t *i) {
	struct expr expr = parse_term(i);

	while (peek_token(*i).type == SPACE_TOKEN && (
		   peek_token(*i + 1).type == GREATER_OR_EQUAL_TOKEN
		|| peek_token(*i + 1).type == GREATER_TOKEN
		|| peek_token(*i + 1).type == LESS_OR_EQUAL_TOKEN
		|| peek_token(*i + 1).type == LESS_TOKEN)) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_term(i));
		expr.type = BINARY_EXPR;
	}

	return expr;
}

static struct expr parse_equality(size_t *i) {
	struct expr expr = parse_comparison(i);

	while (peek_token(*i).type == SPACE_TOKEN && (
		   peek_token(*i + 1).type == EQUALS_TOKEN
		|| peek_token(*i + 1).type == NOT_EQUALS_TOKEN)) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_comparison(i));
		expr.type = BINARY_EXPR;
	}

	return expr;
}

static struct expr parse_and(size_t *i) {
	struct expr expr = parse_equality(i);

	while (peek_token(*i).type == SPACE_TOKEN && peek_token(*i + 1).type == AND_TOKEN) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_equality(i));
		expr.type = LOGICAL_EXPR;
	}

	return expr;
}

static struct expr parse_or(size_t *i) {
	struct expr expr = parse_and(i);

	while (peek_token(*i).type == SPACE_TOKEN && peek_token(*i + 1).type == OR_TOKEN) {
		(*i)++;
		expr.binary.left_expr = push_expr(expr);
		expr.binary.operator = consume_token(i).type;
		consume_space(i);
		expr.binary.right_expr = push_expr(parse_and(i));
		expr.type = LOGICAL_EXPR;
	}

	return expr;
}

// Recursive descent parsing inspired by the book Crafting Interpreters:
// https://craftinginterpreters.com/parsing-expressions.html#recursive-descent-parsing
static struct expr parse_expression(size_t *i) {
	return parse_or(i);
}

static struct statement *parse_statements(size_t *i, size_t *body_statement_count);

static struct statement parse_while_statement(size_t *i) {
	struct statement statement = {0};
	statement.type = WHILE_STATEMENT;

	consume_space(i);
	statement.while_statement.condition = parse_expression(i);

	statement.while_statement.body_statements = parse_statements(i, &statement.while_statement.body_statement_count);

	return statement;
}

static struct statement parse_if_statement(size_t *i) {
	struct statement statement = {0};
	statement.type = IF_STATEMENT;

	consume_space(i);
	statement.if_statement.condition = parse_expression(i);

	statement.if_statement.if_body_statements = parse_statements(i, &statement.if_statement.if_body_statement_count);

	if (peek_token(*i).type == SPACE_TOKEN) {
		(*i)++;

		consume_token_type(i, ELSE_TOKEN);

		if (peek_token(*i).type == SPACE_TOKEN && peek_token(*i + 1).type == IF_TOKEN) {
			(*i) += 2;

			statement.if_statement.else_body_statement_count = 1;

			statement.if_statement.else_body_statements = push_statement(parse_if_statement(i));
		} else {
			statement.if_statement.else_body_statements = parse_statements(i, &statement.if_statement.else_body_statement_count);
		}
	}

	return statement;
}

static struct variable_statement parse_local_variable(size_t *i) {
	struct variable_statement local = {0};

	size_t name_token_index = *i;
	local.name = consume_token(i).str;

	if (peek_token(*i).type == COLON_TOKEN) {
		(*i)++;

		grug_assert(!streq(local.name, "me"), "The local variable 'me' has to have its name changed to something else, since grug already declares that variable");

		consume_space(i);
		struct token type_token = consume_token(i);
		grug_assert(type_token.type == WORD_TOKEN, "Expected a word token after the colon on line %zu", get_token_line_number(name_token_index));

		local.has_type = true;
		local.type = parse_type(type_token.str);
		grug_assert(local.type != type_resource, "The variable '%s' can't have 'resource' as its type", local.name);
		grug_assert(local.type != type_entity, "The variable '%s' can't have 'entity' as its type", local.name);
	}

	grug_assert(peek_token(*i).type == SPACE_TOKEN, "The variable '%s' was not assigned a value on line %zu", local.name, get_token_line_number(name_token_index));

	consume_space(i);
	consume_token_type(i, ASSIGNMENT_TOKEN);

	grug_assert(!streq(local.name, "me"), "Assigning a new value to the entity's 'me' variable is not allowed");

	consume_space(i);
	local.assignment_expr = push_expr(parse_expression(i));

	return local;
}

static struct global_variable_statement *push_global_variable(struct global_variable_statement global_variable) {
	grug_assert(global_variable_statements_size < MAX_GLOBAL_VARIABLES, "There are more than %d global variables in the grug file, exceeding MAX_GLOBAL_VARIABLES", MAX_GLOBAL_VARIABLES);
	global_variable_statements[global_variable_statements_size] = global_variable;
	return global_variable_statements + global_variable_statements_size++;
}

static struct global_variable_statement parse_global_variable(size_t *i) {
	struct global_variable_statement global = {0};

	size_t name_token_index = *i;
	global.name = consume_token(i).str;

	grug_assert(!streq(global.name, "me"), "The global variable 'me' has to have its name changed to something else, since grug already declares that variable");

	assert_token_type(*i, COLON_TOKEN);
	consume_token(i);

	consume_space(i);
	assert_token_type(*i, WORD_TOKEN);
	struct token type_token = consume_token(i);
	global.type = parse_type(type_token.str);

	grug_assert(global.type != type_resource, "The global variable '%s' can't have 'resource' as its type", global.name);
	grug_assert(global.type != type_entity, "The global variable '%s' can't have 'entity' as its type", global.name);

	grug_assert(peek_token(*i).type == SPACE_TOKEN, "The global variable '%s' was not assigned a value on line %zu", global.name, get_token_line_number(name_token_index));

	consume_space(i);
	assert_token_type(*i, ASSIGNMENT_TOKEN);
	consume_token(i);

	consume_space(i);
	global.assignment_expr = parse_expression(i);

	return global;
}

