#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./memory.h"
#include "./list.h"

struct MyString
{
    char* Characters;
    size_t Length;
};
typedef struct MyString MyString;

MyString* str_Init(MemoryArena* arena, char* characters);
List* str_SplitByChar(MyString* str, char splitChar, MemoryArena* arena);
void str_TrimEndWhiteSpace(MyString* str);
bool str_ToInt(MyString* str, int* number);

#endif