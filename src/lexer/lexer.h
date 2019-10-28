#ifndef LEXER_GUARD_H
#define LEXER_GUARD_H

#include "dfa.h"
#include <stdlib.h>

// Type of a found token; used as its ID.
typedef enum { 
    T_POW,
    T_MUL,
    T_DIV,
    T_ADD,
    T_SUB,
    T_INT,
    T_FLT,
    T_NEWL,
    T_END
} TokenType;

// Context of a self-sufficient instance of the lexical analyzer.
typedef struct _LexerContext {
    // A handle to an instance of the DFA.
    DFA state_machine;
    // Source code of the input
    char *source_code;
    // Current position of the read-head in the source code
    size_t read_head;
    // Whether the end has been reached
    short int is_closed;
} *Lex;

// A token found by the lexer.
typedef struct _LexerToken {
    // Type of the token as its ID
    TokenType id;
    // The retrieved lexeme as-is
    char *lexeme;
} *Token;

// Initialize a new instance of the lexer for the given source code
// and return its context.
Lex lex_init(char *input);
// Get the next available token from the given lexer.
Token next_token(Lex lexer);
// Free the lexer and its contents
void lex_dispose(Lex lexer);

#endif // !LEXER_GUARD_H
