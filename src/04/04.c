#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"

typedef struct NumberRange NumberRange;
struct NumberRange
{
    int Start;
    int End;
};

NumberRange FindRange(MyString* sectionString, MemoryArena* arena);
bool OneRangeFullyContainsOther(NumberRange* first, NumberRange* second);
bool RangesOverlap(NumberRange* first, NumberRange* second);
bool NumberInRange(int number, NumberRange* range);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(10));

    List* fileLines = list_Init(&arena, sizeof(char) * 1000);
    file_read_lines(&arena, fileLines, "../../data/04.txt");

    size_t assignmentRangeContainsCount = 0;
    size_t assignmentRangeOverlapsCount = 0;
    ListNode* head = fileLines->Head;
    while(head != NULL) 
    {
        char* line = head->Data;

        MyString* str = str_Init(&arena, line);

        List* splitResult = str_SplitByChar(str, ',', &arena);
        ListNode* splitNode = splitResult->Head;

        MyString* firstSectionRange = (MyString*)splitNode->Data;
        str_TrimEndWhiteSpace(firstSectionRange);

        MyString* secondSectionRange = (MyString*)splitNode->Next->Data;
        str_TrimEndWhiteSpace(secondSectionRange);

        NumberRange firstRange = FindRange(firstSectionRange, &arena);
        NumberRange secondRange = FindRange(secondSectionRange, &arena);

        if (OneRangeFullyContainsOther(&firstRange, &secondRange))
        {
            assignmentRangeContainsCount++;
        }

        if (RangesOverlap(&firstRange, &secondRange))
        {
            assignmentRangeOverlapsCount++;
        }

        head = head->Next;
    }

    printf("04a: %zu\n", assignmentRangeContainsCount);
    printf("04b: %zu", assignmentRangeOverlapsCount);

    return 1;
}

NumberRange FindRange(MyString* sectionString, MemoryArena* arena)
{
    List* splitResult = str_SplitByChar(sectionString, '-', arena);
    
    NumberRange range;

    MyString* startNumberStr = (MyString*)splitResult->Head->Data;
    str_ToInt(startNumberStr, &range.Start);

    MyString* endNumberStr = (MyString*)splitResult->Head->Next->Data;
    str_ToInt(endNumberStr, &range.End);

    return range;
}

bool OneRangeFullyContainsOther(NumberRange* first, NumberRange* second)
{
    bool IsFullyContained = 
        NumberInRange(first->Start, second) && NumberInRange(first->End, second) ||
        NumberInRange(second->Start, first) && NumberInRange(second->End, first);

    return IsFullyContained;
}

bool RangesOverlap(NumberRange* first, NumberRange* second)
{
    bool OneOverlaps = 
        NumberInRange(first->Start, second) || NumberInRange(first->End, second) ||
        NumberInRange(second->Start, first) || NumberInRange(second->End, first);

    return OneOverlaps;
}

bool NumberInRange(int number, NumberRange* range)
{
    return range->Start <= number && range->End >= number;
}
