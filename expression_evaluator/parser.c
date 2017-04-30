#include "helperlib.h"
#include "lexer.h"
#include "parser.h"

L_Token adwance();
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

L_Token token;

int parse(String filename) {
	FILE *source;
	L_Token res;
	fpos_t before_pos, after_pos;

	if(fopen_s(&source, filename, "r"))
		return -1;
	lex_init(source);

	while(!feof(source)) {
		// Parse each line.
		// token = NULL; //parser(0);
		token = adwance();
		if(token && token->t == T_END)
			break;
		res = parser(0);
		// fgetpos(source, &after_pos);
		if(res == NULL)
			fprintf(stdout, "Skipped\n");

		// For each line, move file pointer to before '\n' and add result.
		// fsetpos(source, &before_pos);
		else if(res->t == T_INT)
			fprintf(stdout, "=%ld\n", res->v.intval);
		else
			fprintf(stdout, "=%lf\n", res->v.fltval);

		// while(token->t != T_NEWL) {
		// 	token = advance();
		// }

		token = NULL;
	}
}

// Wrapper to the advance function.
L_Token adwance() {
	L_Token t;
	t = advance();

	if(t && t->t == T_INT) {
		long int i = atol(t->v.lexeme);
		free(t->v.lexeme);
		t->v.intval = i;
	}
	else if(t && t->t == T_FLT) {
		long double d = atolf(t->v.lexeme);
		free(t->v.lexeme);
		t->v.fltval = d;
	}

	return t;
}

L_Token parser(int rightBP) {
	L_Token temp, left;
	if(!token || token->t == T_NEWL || token->t == T_END)
		return NULL;

	temp = token;
	token = adwance();
	left = nud(temp);

	while(rightBP < token->leftBP) {
		temp = token;
		token = adwance();
		left = led(left, temp);
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
}

// Unary minus.
L_Token nud_sub() {
	L_Token right;

	right = parser(100);
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
