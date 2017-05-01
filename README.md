# expr-eval
Expression Evaluator

This is a small-ish project I did for a course. The specification was quite broad:

 - Build an **expression evaluator** with the precedence order: `** > * > / > + > -` (`**` is a power operator).
 - One or more expressions are put in a file which is passed to the evaluator. Each expresssion is on its own line.
 - The result must be appended to the end of the expression as `<expr>=<result>`.
 - Parantheses are optional.
 - Maintain code-readability and modularity.

The code was written in Visual Studio IDE, so it is supposed to be compiled using a VS compiler. Specifically, this program was tested in Visual Studio 2013 on a 64-bit computer.

As of now, the code is divided into four modules: *parser*, *lexer*, *temfile*, and a *helperlib*.

### Features:
 - Distinguishes between integer and float result.
 - No associativity among operators as of yet. This means that all addition is done before any subtraction, and so on.
 - All operators are left-associative, except power which is right-associative.
 - Line comments are "supported", in that if any invalid token is found, the rest of the expression is ignored. `#` is suggested as a comment identifier.
 - Invalid expressions are left untouched.
 - New lines (`\n`) are preserved.
 - The power operator DOES NOT support fractional exponents as of yet. Any such expression is considered invalid and ignored.
 - Spaces are NOT supported as of yet, although this seems trivial to implement.
 - Parantheses are also NOT supported as of yet, which also seems trivial to implement.

The lexer uses a **DFA** to analyse the input and return a *token* on each call. A *token* is a data structure as such:

	typedef struct L_Token_ {
		enum type {T_POW, T_MUL, T_DIV, T_ADD, T_SUB, T_INT, T_FLT, T_NEWL, T_END} t;
		union value {
			long int intval;
			long double fltval;
			String lexeme;
		} v;
		int leftBP;
	} * L_Token;

It represents whether a token is an operator, an integer, a float, a new line or EOF. The lexer stores the token as a string in `lexeme`, and the parser converts it at will.

File editing is handled by writing all the tokens to a temp file on each lexer call immediately. If an expression is to be ignored, the rest of the line is written to the temp file, and the parser moves on to the next expression. Finally, the temp file contents are copied over the existing file.

The parser takes each token and, foremost, writes it to a temp file. Then it implements the **Top Down Operator Precedence** algorithm to parse the expression. A return of a *token* indicates success, while that of `NULL` indicates a semantic error. The TDOP binding powers of the operators are as follows:

 - Subtraction:     10
 - Addition:		20
 - Division:        30
 - Multiplication:	40
 - Power:           50
 - Unary minus:     100
 - Unary plus:      100
 - New line(`\n`):  0
 - EOF:             0
