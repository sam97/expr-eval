#include <stdio.h>
#include <stdlib.h>
#include "helperlib.h"
#include "lexer.h"

// Private functions declarations.
int is_final(int state);
int transition(char input, int state);
String buffer(char c, enum Action action);
L_Token tokenise(int state, String lexeme);

// Enums for the buffer function.
enum Action {STORE, UPDATE, RETURN};

// Token constructor.
L_Token Token(enum type t, union value v, int leftBP) {
	L_Token token = (L_Token) malloc(sizeof(*token));
	token->t = t;
	token->v = v;
	token->leftBP = leftBP;
	return token;
}

// Edges for the DFA.
const int edges[][8] = {
	               /* [0-9],  '.',  '*',  '/',  '+',  '-',  \n,  EOF */
	/* State 0  */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 1  */  {   2,     3,    5,    7,    8,    9,    10,  11 },
	/* State 2  */  {   2,     3,    0,    0,    0,    0,    0,   0  },
	/* State 3  */  {   4,     0,    0,    0,    0,    0,    0,   0  },
	/* State 4  */  {   4,     0,    0,    0,    0,    0,    0,   0  },
	/* State 5  */  {   0,     0,    6,    0,    0,    0,    0,   0  },
	/* State 6  */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 7  */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 8  */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 9  */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 10 */  {   0,     0,    0,    0,    0,    0,    0,   0  },
	/* State 11 */  {   0,     0,    0,    0,    0,    0,    0,   0  }
};

// Global variables.
static FILE *source;

// Called by other programs to initialise the lexer.
int lex_init(FILE *inputfile) {
	if(inputfile == NULL)
		return -1;

	source = inputfile;
	return 0;
}

// Read a token and move the read-head forward.
L_Token advance() {
	int last_state = 0, curr_state = 1;
	fpos_t last_pos;
	String lexeme = NULL;
	char temp;

	if(fgetpos(source, &last_pos)) {
		free(buffer('\n', RETURN));
		return NULL;
	}

	while(curr_state != 0) {
		if(is_final(curr_state)) {
			last_state = curr_state;
			fgetpos(source, &last_pos);
			lexeme = buffer(0, UPDATE);
		}

		temp = fgetc(source);
		if(feof(source)) {
			// if (last_state == 11)
			// 	break;
			// buffer('\n', STORE);
			curr_state = transition(EOF, curr_state);
		}
		else {
			buffer(temp, STORE);
			curr_state = transition(temp, curr_state);
		}
	}

	if(last_state == 0) {
		free(lexeme);
		fsetpos(source, &last_pos);
		// fprintf(stderr, "[-] Syntax error. Ignoring expression...\n");
		free(buffer('\n', RETURN));
		return NULL;
	}

	lexeme = buffer(0, RETURN);
	fsetpos(source, &last_pos);
	return tokenise(last_state, lexeme);
}

// Check if the state is final.
int is_final(int state) {
	return (state == 2 || (state >= 4 && state <= 11));
}

// Make transition from state based on input, return new state.
int transition(char input, int state) {
	if(input >= '0' && input <= '9')
		return edges[state][0];
	switch(input) {
		case EOF:
			return edges[state][7];
		case '.':
			return edges[state][1];
		case '*':
			return edges[state][2];
		case '/':
			return edges[state][3];
		case '+':
			return edges[state][4];
		case '-':
			return edges[state][5];
		case '\n':
			return edges[state][6];
	}
	return 0;
}

// Store each character in buffer, update the null position,
// and null-terminate at that position before returning.
String buffer(char c, enum Action action) {
	static char *buf;
	static int len;
	static int pos;
	String lexeme;

	if(action == STORE) {
		buf = (char *) realloc(buf, len + 1);
		buf[len] = c;
		len++;
		return NULL;
	}
	
	if(action == UPDATE) {
		pos = len;
		return NULL;
	}

	// RETURN
	buf = (char *) realloc(buf, pos + 1);
	buf[pos] = '\0';
	lexeme = buf;
	buf = NULL;
	len = 0;
	pos = 0;
	return lexeme;
}

L_Token tokenise(int state, String lexeme) {
	L_Token token = NULL;
	union value v;
	// v.intval = 0; //
	v.lexeme = lexeme;

	switch(state) {
		case 2: // int
			// v.intval = atol(lexeme);
			// v.lexeme = lexeme;
			token = Token(T_INT, v, 0);
			return token;
		case 4: // float
			// v.fltval = atolf(lexeme); // From helperlib
			// v.lexeme = lexeme;
			token = Token(T_FLT, v, 0);
			return token;
		case 5: // '*'
			token = Token(T_MUL, v, 40);
			break;
		case 6: // '**'
			token = Token(T_POW, v, 50);
			break;
		case 7: // '/'
			token = Token(T_DIV, v, 30);
			break;
		case 8: // '+'
			token = Token(T_ADD, v, 20);
			break;
		case 9: // '-'
			token = Token(T_SUB, v, 10);
			break;
		case 10: // '\n'
			token = Token(T_NEWL, v, 0);
			break;
		case 11: // EOF
			token = Token(T_END, v, 0);
			break;
	}

	// free(lexeme);
	return token;
}
