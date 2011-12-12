/*
 * test_suite.c
 *
 *  Created on: Dec 12, 2011
 *      Author: dustin
 */
#include "test_suite.h"

typedef struct{
	_Bool (**functions)(void);
	int number;
}test_list;

test_list tests;

void expand_tests(){
	_Bool (**temp_functions)(void) = tests.functions;

	tests.functions = malloc(sizeof(_Bool*)*(tests.number+1));

	int i;
	for(i=0; i < tests.number; i++){
		tests.functions[i] = temp_functions[i];
	}
	free(temp_functions);
}
_Bool test_suite_add_test(_Bool (*test_function)(void)){
	expand_tests();
	tests.functions[tests.number] = *test_function;
	tests.number++;
	return true;
}
_Bool test_suite_execute_tests(){
	_Bool return_value = true;

	printf("----------Test suite begin---------\n");
	int i;
	for(i=0; i < tests.number; i++){
		return_value &= tests.functions[i]();
	}
	//------------------------------

	printf("----------Test suite end-----------\n");
	return return_value;
}


