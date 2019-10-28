#include "lexer.h"
#include "dfa.h"
#include <string.h>
#include <stdlib.h>

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
    
    return lexer;
}

void lex_dispose(Lex lexer) {
    if (!lexer) return;

    free(lexer->source_code);
    free(lexer);
}