#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "helperlib.h"
#include "tempfile.h"
#include "parser.h"

// Internal function declarations.
static L_Token adwance();
static void error_routine();
static L_Token parser(int rightBP);
static L_Token nud(L_Token tok);
static L_Token led(L_Token left, L_Token tok);
static L_Token nud_sub();
static L_Token nud_add();
static L_Token led_sub(L_Token left);
static L_Token led_add(L_Token left);
static L_Token led_mul(L_Token left);
static L_Token led_div(L_Token left);
static L_Token led_pow(L_Token left);

// Internal global variables.
static L_Token token;
static FILE *source;

/*
 * This is the only externally linkable function in this module.
 * Is called with the file name to parse.
 * Returns non-zero on error, zero otherwise.
 * The file is modified accordingly.
 */
int parse(String filename) {
	L_Token res;
	char s[40];

	if(fopen_s(&source, filename, "r"))
		return -1;
	lex_init(source);
	mktmp(); // Initialise temp file.

	while(1) {
		// Parse each line.
		token = adwance();
		if(token == NULL) {
			error_routine();
			continue;
		}
		if(token->t == T_NEWL) {
			// New line found.
			tmpputs("\n");
			continue;
		}
		if(token->t == T_END) // EOF found.
			break;

		res = parser(0);
		if(res == NULL) {
			error_routine();
			continue;
		}

		// Store result in s. This is because at EOF, '\n' is not needed.
		if(res->t == T_INT)
			sprintf_s(s, 40 * sizeof(char), " = %ld", res->v.intval);
		else
			sprintf_s(s, 40 * sizeof(char), " = %lf", res->v.fltval);

		tmpputs(s);
		if(token->t == T_NEWL) // Put new line only if present in file.
			tmpputs("\n");

		free(res);
	}
	free(token);

	// Copy temp file to original.
	fclose(source);

	fprintf(stdout, "[!] Started copying. DO NOT terminate!\n");
	fopen_s(&source, filename, "w");
	tmpcopy(source);
	fclose(source);
	fprintf(stdout, "[!] Finished copying. Exiting...\n");

	tmpclose();

	return 0;
}

// Wrapper to the advance function.
// Writes each token except '\n' and EOF to temp file.
// Converts tokens accordingly and frees the 'lexeme'.
static L_Token adwance() {
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
static void error_routine() {
	String buf = NULL;
	int len = 0;
	char c;

	// TODO: Stop using ungetc. Store the last char in c directly.
	while((c = fgetc(source)) != '\n' && c >= 0) {
		buf = (String) realloc(buf, len+1);
		buf[len] = c;
		++len;
	}
	if (c >= 0) { // If not EOF, put '\n'.
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

/*
 * Implements Top Down Operator Precedence.
 * As per the algo, only supposed to be called
   at the start of expression or sub-expression.
 * Returns NULL on error. All error-handling must be done by 'parse'.
 */
static L_Token parser(int rightBP) {
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

// Null denotation.
// Only called for operators with no left argument.
// If done otherwise, return NULL.
static L_Token nud(L_Token tok) {
	if(tok->t == T_SUB)
		return nud_sub();
	if(tok->t == T_ADD)
		return nud_add();
	if(tok->t == T_INT || tok->t == T_FLT)
		return tok;

	return NULL;
}

// Left Denotation.
// Only called for operators with at least a left argument.
// If done otherwise, return NULL.
static L_Token led(L_Token left, L_Token tok) {
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
// Binding power: 100
static L_Token nud_sub() {
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
// Binding power: 100
static L_Token nud_add() {
	L_Token right;

	right = parser(100);
	// if (right == NULL)
	// 	return NULL;

	return right;
}

// Binary subtraction
// Binding power: 10
static L_Token led_sub(L_Token left) {
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

	// free left
	free(left);

	return right;
}

// Binary addition
// Binding power: 20
static L_Token led_add(L_Token left) {
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

	// free left
	free(left);

	return right;
}

// Binary multiply
// Binding power: 40
static L_Token led_mul(L_Token left) {
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

	// free left
	free(left);

	return right;
}

// Binary divide
// Division of integers can return a float.
// Binding power: 30
static L_Token led_div(L_Token left) {
	L_Token right;

	right = parser(30);
	if (right == NULL)
		return NULL;

	if(right->t == T_FLT) { // right is float
		if(right->v.fltval == 0) {
			// TODO: Destroy right.
			if(token->t == T_NEWL)
				ungetc('\n', source);
			return NULL;
		}

		if(left->t == T_FLT) { // left is float
			right->v.fltval = left->v.fltval / right->v.fltval;
		}
		else { // left is int
			right->v.fltval = left->v.intval / right->v.fltval;
		}
	}
	else { // right is int
		if(right->v.intval == 0){
			if(token->t == T_NEWL)
				ungetc('\n', source);
			return NULL;
		}

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

	// free left
	free(left);

	return right;
}

// Binary power
// Exponent as float not supported.
// Binding power: 50
static L_Token led_pow(L_Token left) {
	L_Token right;

	right = parser(50 - 1); // Lower BP to support right-associativity.
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
	else { // right is float. Exopnent as floats not supported.
		long int ti = (long int)right->v.fltval;
		if(ti != right->v.fltval) {// If exponent has fractional part, then skip.
			if(token->t == T_NEWL)
				ungetc('\n', source);
			return NULL;
		}

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

	// free left
	free(left);

	return right;
}
