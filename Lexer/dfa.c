#include <stdlib.h>
#include <string.h>
#include "dfa/dfa.h"

#define SETSTATE(state, token, next_state, next_edge) \
	dfa->states[state] = mkdfa(token, next_state, next_edge)

static mkdfa(char token, int next_state, DfaEdge next_edge) {
	DfaEdge edge = (DfaEdge)malloc(sizeof(struct DfaEdgeListItem));
	edge->token = token;
	edge->value = next_state;
	edge->next_item = next_edge;
}

DFA init_dfa() {
	DFA dfa = (DFA)malloc(sizeof(struct DFAItem));
	if (dfa == NULL)
		return NULL;

	dfa->states_count = DFA_STATES_COUNT;

	// Initialize the states
	dfa->states = (DfaEdge*)malloc(sizeof(DfaEdge) * dfa->states_count);
	dfa->states[0] = NULL;
	SETSTATE(1, '0', 2, 
		SETSTATE(1, '.', 3, 
			SETSTATE(1, '*', 5, 
				SETSTATE(1, '/', 7, 
					SETSTATE(1, '+', 8, 
						SETSTATE(1, '-', 9, 
							SETSTATE(1, '\n', 10, 
								SETSTATE(1, '\0', 11, 
									SETSTATE(1, ' ', 1, NULL)))))))));
	SETSTATE(2, '0', 2,
		SETSTATE(2, '.', 3, NULL));
	SETSTATE(3, '0', 4, NULL);
	SETSTATE(4, '0', 4, NULL);
	SETSTATE(5, ' ', 5, NULL);
	SETSTATE(6, ' ', 6, NULL);
	SETSTATE(7, ' ', 7, NULL);
	SETSTATE(8, ' ', 8, NULL);
	SETSTATE(9, ' ', 9, NULL);
	SETSTATE(10, ' ', 10, NULL);
	SETSTATE(11, ' ', 11, NULL);
	
	int final_states[] = DFA_FINAL_STATES;
	dfa->final_states = (int*)malloc(sizeof(final_states));
	memcpy(dfa->final_states, final_states, sizeof(final_states));
}


