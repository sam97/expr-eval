#include <stdio.h>
#include <stdlib.h>
#include "parser.h"


int main(int argc, char const *argv[]) {
	char const *name;
	if(argc == 2)
		name = argv[1];
	else
		name = "sample.txt";

	parse(name);

	return 0;
}