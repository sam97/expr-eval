#ifndef DFA_GUARD_H
#define	DFA_GUARD_H

// Input definitions for DFA
#define I_NUMBER '0'
#define I_DOT '.'
#define I_ASTERISK '*'
#define I_SLASH '/'
#define I_PLUS '+'
#define I_MINUS '-'
#define I_NEW_LINE '\n'
#define I_END_OF_STRING '\0'
#define I_SPACE ' '

// States of the DFA. 1 is start state and 0 and 3 are non-final states.
typedef enum _DfaState {
	S_ERROR = 0,
	S_START = 1,
	S_INTEGER = 2,
	S_DOT = 3,
	S_FLOAT = 4,
	S_MULTIPLICATION = 5,
	S_POWER = 6,
	S_DIVISION = 7,
	S_PLUS = 8,
	S_MINUS = 9,
	S_NEW_LINE = 10, // TODO: Why this in an interpreter?
	S_EOD = 11
} DfaState;

#define DFA_STATES_COUNT 12

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
};
typedef struct DFAItem *DFA;

// Initialize the DFA for the expression evaluator grammar
// and return a handle to it.
DFA init_dfa();

// Returns the next state, or -1 if errored out.
int get_next_state(DFA dfa, DfaState current_state, char input);

int is_final_state(DFA dfa, DfaState state);

//TokenType getTokenType(DFA* dfa, int state);

// Edges for the DFA.
// State 0 is the error state. Any state that matches unknown characters is
// forwarded here.
// State 1 is the start state. Usually this state will determine which final
// state the FSM will end up in.
// State 2 - Integer
// State 3 - Dot after an integer. Intermediate state between Integer and Float
// State 4 - Floating decimal
// State 6 - '**'
// Rest of the states can be determined by looking at State 1.
// TODO: Bug in state 5. '* *' will be interpreted as State 6.
// 
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
