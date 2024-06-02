#include "grug/grug.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <grug_path> <dll_path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *grug_path = argv[1];
	char *dll_path = argv[2];

	// printf("grug_path: %s\ndll_path: %s\n", grug_path, dll_path);

	if (grug_test_regenerate_dll(grug_path, dll_path)) {
		fprintf(stderr, "%s in %s:%d\n", grug_error.msg, grug_error.filename, grug_error.line_number);
		exit(EXIT_FAILURE);
	}
}
