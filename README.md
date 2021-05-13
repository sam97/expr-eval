# expr-eval
Expression Evaluator

### Update 2021-05-13

Here is some life advice to follow:

- Don't use your employer's machine to work on personal projects.
- Push your commits to the remote repo regularly. Yes, even if they're intermediary commits. You can clean them up later.
- If you happen to use your employer's machine to work on personal projects, be sure to move them to your personal machine at the earliest.
  - (Get a personal machine at the earliest.)
- When you submit your employer's machine during exit, **make sure you've followed all of the advice above**!

As you can imagine, I didn't.

~~I'm archiving this project because~~ I lost more than a year's worth of commits. I was naive enough to work on it on my employer's machine, not push ~45 commits, forget to move the code once I had my personal PC built, and then return the machine thinking that I could always clone the code from GitHub (to which I hadn't pushed).

The progress I'd made was amazing. Some of it is still availabe in the [v2 branch](../../tree/v2.0.0-alpha/):
- I managed to move away from both MSVC and the one-file-structure.
- I retargeted to MinGW GCC and reconfigured for VS Code (not the best IDE in hindsight, but it gets the job done).
- I integrated CMake for a saner experience (although that is still debatable).
- With a lot of courage and self-motivation, I finally added tests! (Although CMocka now seems a very average choice.)
- I separated the lexer into its own library with its own tests.

In the missing commits I'd gone pretty far:
- The DFA was extracted from the lexer and made its own library to reuse in the parser.
- Oh yes, the parser was almost complete. I recall that a few testcases were failing, but I'd finally understood and implemented a decent Pratt parser.
- Small but time-taking optimisations, like token-folding (??) that I barely remember.
- I had a very minimal AST going. No error-handling yet, but it was going somewhere.
- Test cases. Lots and lots of test cases. For all the libraries.
- A more complex CMake setup to account for individual and `all` builds, fine-tuned compiler configurations, and CMake Test.

I had a lot planned, as well:
- Valgrind was sorely needed. CMocka has some memory-leak detection, but it's very patchy and it didn't work as intended sometimes.
- The original idea had mutated to build a Pratt-parser-generator that uses macros, using which the evaluator will be built, but I hadn't worked it out yet.
- A better testing framework. CMocka was alright but memory leak detection was a pain point.
- I'd even planned to build an interactive webpage for the evaluator using WASM. For learning purposes, obviously.

I was going to archive this project because when I realised that a lot of my progress was gone I was completely discouraged. But as I write this I'm feeling hopeful that I can rebound with better quality. Let's see if I can manage that. (After I find a job, of course.)

Lesson learnt: keep your work and personal projects separate.

----

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
