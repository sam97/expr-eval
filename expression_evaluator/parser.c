#include <stdio.h>
#include <stdlib.h>
#include "helperlib.h"
#include "lexer.h"
#include "parser.h"

L_Token adwance();
void error_routine();
L_Token parser(int rightBP);
L_Token nud(L_Token tok);
L_Token led(L_Token left, L_Token tok);
L_Token nud_sub();
L_Token nud_add();
L_Token led_sub(L_Token left);
L_Token led_add(L_Token left);
L_Token led_mul(L_Token left);
L_Token led_div(L_Token left);
L_Token led_pow(L_Token left);

static L_Token token;
static FILE *source;

int parse(String filename) {
	L_Token res;
	char s[40];

	if(fopen_s(&source, filename, "r"))
		return -1;
	// Initialize lexer.
	lex_init(source);
	// Initialise temp file.
	mktmp();

	while(!feof(source)) {
		// Parse each line.
		token = adwance();
		if(token == NULL) {
			error_routine();
			continue;
		}
		if(token->t == T_NEWL) {
			tmpputs("\n");
			continue;
		}
		if(token->t == T_END)
			break;

		res = parser(0);
		if(res == NULL) {
			error_routine();
			continue;
		}

		if(res->t == T_INT)
			sprintf_s(s, 40 * sizeof(char), "=%ld", res->v.intval);
		else
			sprintf_s(s, 40 * sizeof(char), "=%lf", res->v.fltval);

		tmpputs(s);
		if(token->t == T_NEWL)
			tmpputs("\n");
	}

	// Copy file.
	fclose(source);

	fopen_s(&source, filename, "w");
	tmpcopy(source);
	fclose(source);

	tmpclose();
}

// Wrapper to the advance function.
L_Token adwance() {
	L_Token t;
	t = advance();
	if(!t)
		return NULL;

	if(t->t == T_INT) {
		long int i = atol(t->v.lexeme);
		tmpputs(t->v.lexeme);
		free(t->v.lexeme);
		t->v.intval = i;
	}
	else if(t->t == T_FLT) {
		long double d = atolf(t->v.lexeme);
		tmpputs(t->v.lexeme);
		free(t->v.lexeme);
		t->v.fltval = d;
	}
	else {
		if(t->t != T_NEWL && t->t != T_END)
			tmpputs(t->v.lexeme);
		free(t->v.lexeme);
		t->v.lexeme = NULL;
	}

	return t;
}

// Writes rest of the line to tmpfile.
void error_routine() {
	String buf = NULL;
	int len = 0;
	char c;

	while((c = fgetc(source)) != '\n' && c >= 0) {
		buf = (String) realloc(buf, len+1);
		buf[len] = c;
		++len;
	}
	if (c >= 0) {
		buf = (String)realloc(buf, len + 1);
		buf[len] = '\n';
		++len;
	}
	buf = (String)realloc(buf, len + 1);
	buf[len] = '\0';
	++len;

	tmpputs(buf);
	free(buf);
}

L_Token parser(int rightBP) {
	L_Token temp, left;
	if(!token || token->t == T_END)
		return NULL;
	if(token->t == T_NEWL) {
		ungetc('\n', source);
		return NULL;
	}

	temp = token;

	token = adwance();
	if(token == NULL)
		return NULL;

	left = nud(temp);
	if(left == NULL)
		return NULL;

	while(rightBP < token->leftBP) {
		temp = token;

		token = adwance();
		if(token == NULL || token->t == T_END)
			return NULL;
		if(token->t == T_NEWL) {
			ungetc('\n', source);
			return NULL;
		}
		
		left = led(left, temp);
		if(left == NULL)
			return NULL;
	}

	return left;
}

L_Token nud(L_Token tok) {
	if(tok->t == T_SUB)
		return nud_sub();
	if(tok->t == T_ADD)
		return nud_add();
	if(tok->t == T_INT || tok->t == T_FLT)
		return tok;

	return NULL;
}

L_Token led(L_Token left, L_Token tok) {
	switch(tok->t) {
		case T_SUB:
			return led_sub(left);
		case T_ADD:
			return led_add(left);
		case T_MUL:
			return led_mul(left);
		case T_DIV:
			return led_div(left);
		case T_POW:
			return led_pow(left);
	}

	return NULL;
}

// Unary minus.
L_Token nud_sub() {
	L_Token right;

	right = parser(100);
	if (right == NULL)
		return NULL;

	if(right->t == T_INT)
		right->v.intval = -(right->v.intval);
	if(right->t == T_FLT)
		right->v.fltval = -(right->v.fltval);

	return right;
}

// Unary plus.
L_Token nud_add() {
	L_Token right;

	right = parser(100);
	if (right == NULL)
		return NULL;
	// if(right->t == T_INT)
	// 	right->v.intval = -(right->v.intval);
	// if(right->t == T_FLT)
	// 	right->v.fltval = -(right->v.fltval);

	return right;
}

// Binary minus
L_Token led_sub(L_Token left) {
	L_Token right;

	right = parser(10);
	if (right == NULL)
		return NULL;

	if(left->t == T_FLT) { // left is float
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.fltval - right->v.fltval;
		}
		else { // right is int
			right->v.fltval = left->v.fltval - right->v.intval;
			right->t = T_FLT;
		}
	}
	else { // left is int
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.intval - right->v.fltval;
		}
		else { // right is int
			right->v.intval = left->v.intval - right->v.intval;
		}
	}

	// free left???

	return right;
}

// Binary plus
L_Token led_add(L_Token left) {
	L_Token right;

	right = parser(20);
	if (right == NULL)
		return NULL;

	if(left->t == T_FLT) { // left is float
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.fltval + right->v.fltval;
		}
		else { // right is int
			right->v.fltval = left->v.fltval + right->v.intval;
			right->t = T_FLT;
		}
	}
	else { // left is int
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.intval + right->v.fltval;
		}
		else { // right is int
			right->v.intval = left->v.intval + right->v.intval;
		}
	}

	return right;
}

// Binary multiply
L_Token led_mul(L_Token left) {
	L_Token right;

	right = parser(40);
	if (right == NULL)
		return NULL;

	if(left->t == T_FLT) { // left is float
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.fltval * right->v.fltval;
		}
		else { // right is int
			right->v.fltval = left->v.fltval * right->v.intval;
			right->t = T_FLT;
		}
	}
	else { // left is int
		if(right->t == T_FLT) { // right is float
			right->v.fltval = left->v.intval * right->v.fltval;
		}
		else { // right is int
			right->v.intval = left->v.intval * right->v.intval;
		}
	}

	return right;
}

// Binary divide
L_Token led_div(L_Token left) {
	L_Token right;

	right = parser(30);
	if (right == NULL)
		return NULL;

	if(right->t == T_FLT) { // right is float
		if(right->v.fltval == 0)
			// TODO: Destroy right.
			return NULL;

		if(left->t == T_FLT) { // left is float
			right->v.fltval = left->v.fltval / right->v.fltval;
		}
		else { // left is int
			right->v.fltval = left->v.intval / right->v.fltval;
		}
	}
	else { // right is int
		if(right->v.intval == 0)
			return NULL;

		if(left->t == T_FLT) { // left is float
			right->v.fltval = left->v.fltval / right->v.intval;
			right->t = T_FLT;
		}
		else { // left is int
			long int ti;
			long double td;
			td = (long double)left->v.intval / right->v.intval;
			ti = (long int)td;

			if(ti == td) {
				right->v.intval = ti;
			}
			else {
				right->v.fltval = td;
				right->t = T_FLT;
			}
		}
	}

	return right;
}

// Binary power
L_Token led_pow(L_Token left) {
	L_Token right;

	right = parser(50 - 1);
	if (right == NULL)
		return NULL;

	if(right->t == T_INT) { // right is int.
		if(left->t == T_INT) { // left is int.
			long double t = pow((long double)left->v.intval, right->v.intval);
			
			if(right->v.intval < 0) { // negative exponent.
				right->v.fltval = t;
				right->t = T_FLT;
			}
			else {
				right->v.intval = (long int)t;
			}
		}
		else { // left is float.
			right->v.fltval = pow(left->v.fltval, right->v.intval);
			right->t = T_FLT;
		}
	}
	else { // right is float. Floats as exopnent not supported.
		long int ti = (long int)right->v.fltval;
		if(ti != right->v.fltval) // If exponent has fractional part, then skip.
			return NULL;

		if(left->t == T_INT) {// left is int.
			long double t = pow(left->v.intval, ti);
			
			if(ti < 0) { // negative exponent.
				right->v.fltval = t;
			}
			else {
				right->v.intval = (long int)t;
				right->t = T_INT;
			}
		}
		else { // left is float.
			right->v.fltval = pow(left->v.fltval, ti);
		}
	}

	return right;
}
