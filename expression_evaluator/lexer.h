#ifndef LEXER_LIB_GUARD
#define LEXER_LIB_GUARD

#include <stdio.h>
#include <stdlib.h>

// Token data structure.
typedef struct L_Token_ {
	enum type {T_POW, T_MUL, T_DIV, T_ADD, T_SUB, T_INT, T_FLT, T_NEWL, T_END} t;
	// Make this union anonymous?
	union value {
		long int intval;
		long double fltval;
	} v;
	int leftBP;
} * L_Token;

int lex_init(FILE *inputfile);
L_Token advance();

#endif