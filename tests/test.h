#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static inline bool assert_str_equal(char *input, char *correct)
{
	if (strncmp(input, correct, strlen(correct)) != 0)
	{
		printf("Error: expected '%s', got '%s'\n", correct, input);
		return false;
	}
	return true;
}

static inline bool assert_int_equal(int input, int correct)
{
	if (input != correct)
	{
		printf("Error: expected %i, got %i\n", correct, input);
		return false;
	}
	return true;
}
