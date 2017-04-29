#include <stdio.h>
#include <stdlib.h>
#include "helperlib.h"
#include "lexer.h"

#define _CRT_SECURE_NO_WARNINGS


int main(int argc, char const *argv[]) {
	char fname[10];
	FILE *source;
	L_Token token = NULL;
	long double ld;

	ld = atolf("123.45654236");
	fprintf(stdout, "=%lf", ld);
	getchar();
	/*
	printf("Enter the filename: \n");
	fgets(fname, 10, stdin);
	fname[8] = '\0';

	fopen_s(&source, fname, "r");
	lex_init(source);

	while(1) {
		token = advance();
		if (token == NULL || token->t == T_END) {
			printf("END ");
			break;
		}
		switch (token->t) {
		case T_MUL:
			printf("T_MUL ");
			break;
		case T_ADD:
			printf("T_ADD ");
			break;
		case T_SUB:
			printf("T_SUB ");
			break;
		case T_POW:
			printf("T_POW ");
			break;
		case T_DIV:
			printf("T_DIV ");
			break;
		case T_INT:
			printf("T_INT(%ld) ", token->v.intval);
			break;
		case T_FLT:
			printf("T_FLT(%Lf) ", token->v.fltval);
			break;
		case T_NEWL:
			printf("T_NEWL ");
			break;
		case T_END:
			printf("T_END ");
			break;
		default:
			printf("ERR ");
		}
	}
	getchar();
	*/
	return 0;
}