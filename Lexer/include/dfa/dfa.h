#ifndef DFA_GUARD_H
#define	DFA_GUARD_H

//#include "lexer.h";

#define DFA_STATES_COUNT 12
#define DFA_FINAL_STATES { 2, 4, 5, 6, 7, 8, 9, 10, 11 }

struct DfaEdgeListItem {
	// The token that this entry represents
	char token;
	// Next State
	int value;
	struct DfaEdgeListItem *next_item;
};
typedef struct DfaEdgeListItem *DfaEdge;

struct DFAItem {
	DfaEdge* states;
	int states_count;
	int* final_states;
	int final_states_count;
};
typedef struct DFAItem *DFA;

DFA init_dfa();
int getNextState(DFA dfa);
int isFinalState(DFA dfa, int state);
//TokenType getTokenType(DFA* dfa, int state);

#endif // !DFA_GUARD_H
