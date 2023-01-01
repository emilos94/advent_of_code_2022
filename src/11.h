#ifndef DAY_11_H
#define DAY_11_H

#include "./util/arraylist.h"
#include "./util/memory.h"
#include "./util/my_string.h"
#include <stdbool.h>

#define ULLONG unsigned long long

struct Monkey
{
    size_t Index;
    List* Items;
    
    size_t DivisibleBy;
    size_t MonkeyIndexTrue;
    size_t MonkeyIndexFalse;

    MyString* Left;
    char Operator;
    MyString* Right;

    size_t InspectCount;
};
typedef struct Monkey Monkey;

int eleven_solve(MemoryArena* arena, List* fileLines);

ArrayList* DeepCopyMonkeyList(MemoryArena* arena, ArrayList* monkeyList);
size_t FindMonkeyBusinessValue(MemoryArena* arena, ArrayList* monkeyList, size_t numRounds, bool shouldDivideByThree);
ULLONG GetMonkeyTestValue(ULLONG* item, MyString* value);
ArrayList* ParseMonkeys(MemoryArena* arena, List* fileLines);
bool CompareMonkiesByInspectCount(void*, void*);
void PrettyPrintMonkey(Monkey* monkey);
#endif