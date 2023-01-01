#ifndef MY_STRING_TESTS_H
#define MY_STRING_TESTS_H

#include "../util/my_string.h"
#include "../util/memory.h"
#include <assert.h>

int my_string_tests_run(void);

void test_SplitByChar_EndsWithSplit(MemoryArena* arena);
void test_SplitByChar(MemoryArena* arena);

#endif