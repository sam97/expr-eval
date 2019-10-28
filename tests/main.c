#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdio.h>
#include "dfa_tests.h"
#include "lexer_tests.h"

// Main function that is supposed to run all the tests at once.
// Idea is to write test cases for each module in a separate file,
// import it here and run the tests. CMocka will handle the rest.
int main()
{
	printf("[+] Beginning tests...\n");

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_dfa_init),
		cmocka_unit_test(test_get_next_state),
		cmocka_unit_test(test_is_final_state),
		
		cmocka_unit_test(test_lexer_init),
		cmocka_unit_test(test_lexer_next)
	};

	cmocka_set_test_filter("test_*");
	int failed_tests = -1;
	if ((failed_tests = cmocka_run_group_tests(tests, NULL, NULL))) {
		printf("[-] %d Tests Failed.\n", failed_tests);
	}
	else {
		printf("[+] Tests complete.\n");
	}

	return 0;
}
