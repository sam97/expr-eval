#include <stdlib.h>
#include <string.h>
#include "dfa.h"

#define SETSTATE(state, token, next_state, next_edge) \
	dfa->states[state] = mkdfa(token, next_state, next_edge)

static DfaEdge mkdfa(char token, int next_state, DfaEdge next_edge) {
	DfaEdge edge = (DfaEdge)malloc(sizeof(struct DfaEdgeListItem));
	if (edge == NULL)
		return NULL;

	edge->token = token;
	edge->value = next_state;
	edge->next_item = next_edge;
	return edge;
}

DFA init_dfa() {
	DFA dfa = (DFA)malloc(sizeof(struct DFAItem));
	if (dfa == NULL)
		return NULL;

	dfa->states_count = DFA_STATES_COUNT;

	// Initialize the states
	dfa->states = (DfaEdge*)malloc(sizeof(DfaEdge) * dfa->states_count);
	if (dfa->states == NULL) {
		free(dfa);
		return NULL;
	}

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
	
	// Populate the final states
	int final_states[] = DFA_FINAL_STATES;
	dfa->final_states = (int*)malloc(sizeof(final_states));
	if (dfa->final_states == NULL) {
		for (size_t i = 0; i < dfa->states_count; i++)
		{
			DfaEdge edge = dfa->states[i];
			while (edge != NULL) {
				DfaEdge next = edge->next_item;
				free(edge);
				edge = next;
			}
		}
		free(dfa);
		return NULL;
	}

	dfa->final_states_count = (sizeof(final_states)/sizeof(final_states[0]));
	memcpy(dfa->final_states, final_states, sizeof(final_states[0]) * dfa->final_states_count);

	return dfa;
}

int get_next_state(DFA dfa, int current_state, char input) {
	if (!dfa)
		return -1;
	if (current_state >= dfa->states_count)
		return -1;
	
	input = input >= '0' && input <= '9' ? '0' : input;

	DfaEdge edge = dfa->states[current_state];
	while (edge) {
		if (edge->token == input)
			return edge->value;
		edge = edge->next_item;
	}

	return 0;
}

int is_final_state(DFA dfa, int state) {
	if (!dfa || state >= dfa->states_count)
		return -1;
	
	for (size_t i = 0; i < dfa->final_states_count; i++)
	{
		if (dfa->final_states[i] == state)
			return 1;
	}

	return 0;
}
