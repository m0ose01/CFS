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

