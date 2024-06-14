#include "grug/grug.h"

#include <stdio.h>
#include <stdlib.h>

void init() {
	grug_init((grug_function_t[]){
		{ .name="foo", .return_type="i32", .arguments=(grug_argument_t[]){ { .name="a", .type="i64" }, { .name="b", .type="i64" }, { 0 } } },
		{ .name="bar", .return_type="f32", .arguments=(grug_argument_t[]){ { .name="x", .type="i32" }, { 0 } } },
		{ 0 }
	});
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <grug_path> <dll_path> <c_path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	init();

	char *grug_path = argv[1];
	char *dll_path = argv[2];
	char *c_path = argv[3];

	if (grug_test_regenerate_dll(grug_path, dll_path, c_path)) {
		fprintf(stderr, "%s\n", grug_error.msg);
		exit(EXIT_FAILURE);
	}
}
