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
MyString* str_InitLimit(MemoryArena* arena, char* characters, size_t length);
void str_Print(MyString* str);
void str_PrintLine(MyString* str);
void str_PrintSubString(MyString* str, size_t start, size_t end);
bool str_Equals(MyString* either, MyString* other);

List* str_SplitByChar(MyString* str, char splitChar, MemoryArena* arena);
void str_TrimEndWhiteSpace(MyString* str);
bool str_ToInt(MyString* str, int* number);
bool str_ToSizeT(MyString* str, size_t* number);
bool str_ContainsChar(MyString* str, char c);
bool str_StartsWith(MyString* str, char* c, size_t length);

#endif