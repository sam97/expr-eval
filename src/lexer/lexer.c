#include "lexer.h"
#include "dfa.h"
#include <string.h>
#include <stdlib.h>

static char *strcatchar(char *string, char c, size_t str_len);
static Token tokenise(DfaState final_state, char *lexeme);

Lex lex_init(char *input) {
    if (!input)
        return NULL;

    Lex lexer = (Lex)malloc(sizeof(struct _LexerContext));
    if (!lexer)
        return NULL;
    
    lexer->is_closed = 0;
    lexer->read_head = 0;
    lexer->state_machine = init_dfa();

    size_t length = strlen(input);
    lexer->source_code = (char *)malloc(sizeof(char) * (length + 1));
    if (!lexer->source_code) {
        free(lexer);
        return NULL;
    }
    memcpy(lexer->source_code, input, sizeof(char) * length);
    lexer->source_code[length] = '\0';
    lexer->source_length = length;
    
    return lexer;
}

Token next_token(Lex lexer) {
    if (!lexer) {
        return NULL;
    }
    
    char *source = lexer->source_code;
    int position = lexer->read_head;
    DfaState curr_state = S_START, final_state = S_ERROR;
    size_t source_len = lexer->source_length,
        lexeme_len = 0;
    char *lexeme = NULL;
    Token token = (Token)malloc(sizeof(*token));
    
    if (!token || lexer->is_closed) goto error;

    while (curr_state != S_ERROR) {
        if (position > source_len)
            break;

        // Get next character and transition states.
        char input = source[position];
        curr_state = get_next_state(lexer->state_machine, curr_state, input);
        if ((int)curr_state < 0) {
            curr_state = S_ERROR;
            // Eat the error
            // final_state = 0;
        }

        if (is_final_state(lexer->state_machine, curr_state)) {
            final_state = curr_state;
        }

        // Store lexeme of the token.
        if (curr_state != S_ERROR) {
            lexeme = strcatchar(lexeme, input, lexeme_len);
            ++lexeme_len;
        }

        ++position;
    }
    --position;

    // Syntax error-handling
    if (final_state == S_ERROR) {
    error:
        // TODO: Give more error info. Use lexeme if needed.
        return tokenise(S_ERROR, lexeme);
    }

    // Finish building lexeme
    if (curr_state != S_EOD) {
        lexeme = strcatchar(lexeme, '\0', lexeme_len);
        ++lexeme_len;
    }

    lexer->read_head = position;
    if (curr_state == S_EOD) {
        lexer->is_closed = 1;
    }
    
    return tokenise(final_state, lexeme);
}

void lex_dispose(Lex lexer) {
    if (!lexer) return;

    free(lexer->source_code);
    free(lexer);
}

void token_dispose(Token token) {
    if (!token) return;

    free(token->lexeme);
    free(token);
}

// Append character to string. Doesn't null-terminate.
static char *strcatchar(char *string, char c, size_t str_len) {
    string = (char *)realloc(string, (str_len + 1) * sizeof(char));
    if (!string) return NULL;
    string[str_len] = c;
    return string;
}

static Token tokenise(DfaState final_state, char *lexeme) {
    Token token = (Token)malloc(sizeof(*token));
    if (!token) return NULL;
    token->lexeme = lexeme;

    TokenType type;
    switch (final_state)
	{
	case S_INTEGER:
        type = T_INT; break;
	case S_FLOAT:
        type = T_FLT; break;
	case S_MULTIPLICATION:
    case S_MULT_SPACED:
        type = T_MUL; break;
	case S_POWER:
        type = T_POW; break;
	case S_DIVISION:
        type = T_DIV; break;
	case S_PLUS:
        type = T_ADD; break;
	case S_MINUS:
        type = T_SUB; break;
	case S_NEW_LINE:
        type = T_NEWL; break;
	case S_EOD:
        type = T_END; break;
	default:
		type = T_ERR;
	}

    token->id = type;
    return token;
}
