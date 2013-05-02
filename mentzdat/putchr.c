#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int v; // parameter value
	int i; // loop index

	if (argc < 2) return 1; // Not enough parameter

	for (i = 1; i < argc; ++i) {
		v = atoi(argv[i]) % 256;
		printf("%c", (unsigned char)v);
	}
	return 0;
}
