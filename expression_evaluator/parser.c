#include "helperlib.h"
#include "lexer.h"
#include "parser.h"

int parse(String filename) {
	FILE *source;
	L_Token token;
	fpos_t before_pos, after_pos;

	if(fopen_s(filename, &source, "r+w"))
		return -1;

	while(!feof(source)) {
		// Parse each line.
		token = parser(0, &before_pos);
		fgetpos(source, &after_pos);
		if(token == NULL)
			return 1;

		// For each line, move file pointer to before '\n' and add result.
		fsetpos(source, &before_pos);
		if(token->t == T_INT)
			fprintf(stdout, "=%ld", token->v.intval);
		else
			fprintf(stdout, "=%lf", token->v.fltval);

		while(token->t != T_NEWL) {
			token = advance();
		}
	}
}
