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

// Edges for the DFA.
// const int edges[][9] = {
// 	               /* [0-9],  '.',  '*',  '/',  '+',  '-',  \n,  EOF,  ' '  */
// 	/* State 0  */  {   0,     0,    0,    0,    0,    0,    0,   0,    0   },
// 	/* State 1  */  {   2,     3,    5,    7,    8,    9,    10,  11,   1   },
// 	/* State 2  */  {   2,     3,    0,    0,    0,    0,    0,   0,    0   },
// 	/* State 3  */  {   4,     0,    0,    0,    0,    0,    0,   0,    0   },
// 	/* State 4  */  {   4,     0,    0,    0,    0,    0,    0,   0,    0   },
// 	/* State 5  */  {   0,     0,    6,    0,    0,    0,    0,   0,    5   },
// 	/* State 6  */  {   0,     0,    0,    0,    0,    0,    0,   0,    6   },
// 	/* State 7  */  {   0,     0,    0,    0,    0,    0,    0,   0,    7   },
// 	/* State 8  */  {   0,     0,    0,    0,    0,    0,    0,   0,    8   },
// 	/* State 9  */  {   0,     0,    0,    0,    0,    0,    0,   0,    9   },
// 	/* State 10 */  {   0,     0,    0,    0,    0,    0,    0,   0,    10  },
// 	/* State 11 */  {   0,     0,    0,    0,    0,    0,    0,   0,    11  }
// };

#endif // !DFA_GUARD_H
