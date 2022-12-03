#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"

#define ASCII_A 65
#define ASCII_Z 90

#define ASCII_a 97
#define ASCII_z 122

size_t FindLineLength(char* line);
char FindSharedItemType(char* line, size_t lineLength);
size_t MapToPriority(char c);
size_t FindGroupBadge(
    char* lineOne, size_t lineLengthOne, 
    char* lineTwo, size_t lineLengthTwo, 
    char* lineThree, size_t lineLengthThree
);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(10));

    List* fileLines = list_Init(&arena, sizeof(char) * 1000);
    file_read_lines(&arena, fileLines, "../../data/03.txt");

    size_t itemPrioritySum = 0;
    ListNode* head = fileLines->Head;
    while(head != NULL) 
    {
        char* line = head->Data;

        size_t lineLength = FindLineLength(line);
        char commonItemType = FindSharedItemType(line, lineLength);
        itemPrioritySum += MapToPriority(commonItemType);

        head = head->Next;
    }

    size_t badgePrioritySum = 0;
    ListNode* first = fileLines->Head;
    while (first != NULL)
    {
        char* lineOne = first->Data;
        size_t lineLengthOne = FindLineLength(lineOne);
        first = first->Next;

        char* lineTwo = first->Data;
        size_t lineLengthTwo = FindLineLength(lineTwo);
        first = first->Next;

        char* lineThree = first->Data;
        size_t lineLengthThree = FindLineLength(lineThree);

        char groupBadge = FindGroupBadge(
            lineOne, lineLengthOne,
            lineTwo, lineLengthTwo,
            lineThree, lineLengthThree
        );
        badgePrioritySum += MapToPriority(groupBadge);

        first = first->Next;
    }
    
    printf("03a: %zu\n", itemPrioritySum);
    printf("03b: %zu", badgePrioritySum);
}


size_t FindGroupBadge(
    char* lineOne, size_t lineLengthOne, 
    char* lineTwo, size_t lineLengthTwo, 
    char* lineThree, size_t lineLengthThree)
{
    for (size_t i = 0; i < lineLengthOne; i++) 
    {
        char currentChar = lineOne[i];

        for (size_t j = 0; j < lineLengthTwo; j++)
        {
            if (currentChar != lineTwo[j]) continue;
            
            for (size_t k = 0; k < lineLengthThree; k++) 
            {
                if (currentChar == lineThree[k])
                {
                    return currentChar;
                }
            }
        }
    }

    assert(false);
}

size_t FindLineLength(char* line)
{
    size_t lineLength = 0;
    char currentChar = line[lineLength];
    while(currentChar != '\n' && currentChar != '\0')
    {
        currentChar = line[lineLength++];
    }
    lineLength--;
    return lineLength;
}

char FindSharedItemType(char* line, size_t lineLength)
{
    for (size_t i = 0; i < lineLength / 2; i++)
    {
        char currentChar = line[i];
        for (size_t j = lineLength / 2; j < lineLength; j++)
        {
            if (currentChar == line[j]) return currentChar;
        }
    }

    assert(false);
}

size_t MapToPriority(char c)
{
    int asciiValue = (int)c;
    bool isUpperCase = asciiValue >= ASCII_A && asciiValue <= ASCII_Z;
    if (isUpperCase) return asciiValue - 38; // maps to 27-52

    bool isLowerCase = asciiValue >= ASCII_a && asciiValue <= ASCII_z;
    if (isLowerCase) return asciiValue - 96; // maps to 1-26

    assert(false);
}