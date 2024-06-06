#include "grug/grug.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <grug_path> <dll_path> <c_path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *grug_path = argv[1];
	char *dll_path = argv[2];
	char *c_path = argv[3];

	grug_init((grug_mod_types_t){.a=42});

	if (grug_test_regenerate_dll(grug_path, dll_path, c_path)) {
		fprintf(stderr, "%s\n", grug_error.msg);
		exit(EXIT_FAILURE);
	}
}
