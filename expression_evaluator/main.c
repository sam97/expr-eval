#include <stdio.h>
#include <stdlib.h>
#include "helperlib.h"
#include "lexer.h"
#include "parser.h"

#define _CRT_SECURE_NO_WARNINGS


int main(int argc, char const *argv[]) {
	// char c, fname[10];
	FILE *source, *dest;
	L_Token token = NULL;
	long double ld;

	// tmpfile_s(&source);
	// fopen_s(&dest, "test1.txt", "r+");

	// fprintf(source, "%s\r\n", "abcdefghijklm");
	// fprintf(source, "%s\r\n", "efghijklmnopq");
	// rewind(source);

	// rewind(dest);
	// while((c = fgetc(source)) != EOF) {
	// 	fputc(c, dest);
	// 	fflush(dest);
	// }

	// fclose(source);
	// fclose(dest);

	// ld = atolf("123.45654236");
	// fprintf(stdout, "=%lf", ld);
	// getchar();
	
	// printf("Enter the filename: \n");
	// fgets(fname, 10, stdin);
	// fname[8] = '\0';

	parse("test.txt");

	// fopen_s(&source, "test.txt", "r");
	// lex_init(source);

	// while(1) {
	// 	token = advance();
	// 	if (token == NULL || token->t == T_END) {
	// 		printf("END(%s) ", token->v.lexeme);
	// 		break;
	// 	}
	// 	switch (token->t) {
	// 	case T_MUL:
	// 		printf("T_MUL(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_ADD:
	// 		printf("T_ADD(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_SUB:
	// 		printf("T_SUB(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_POW:
	// 		printf("T_POW(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_DIV:
	// 		printf("T_DIV(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_INT:
	// 		printf("T_INT(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_FLT:
	// 		printf("T_FLT(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_NEWL:
	// 		printf("T_NEWL(%s) ", token->v.lexeme);
	// 		break;
	// 	case T_END:
	// 		printf("T_END(%s) ", token->v.lexeme);
	// 		break;
	// 	default:
	// 		printf("ERR ");
	// 	}
	// }

	// token = advance();
	
	getchar();
	
	return 0;
}