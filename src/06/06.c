#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"

bool WindowPriorCharsDiffer(MyString* str, size_t index, size_t windowSize);
size_t FindStartOfDataPacket(MyString* str, size_t windowSize);

int main(void)
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Kilobytes(10));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../../data/06.txt", 10000);
    list_Loop(fileLines, MyString, listNode, lineStr)
    {
        size_t packetStart = FindStartOfDataPacket(lineStr, 3);
        printf("06a: %lu\n", packetStart);

        size_t messageStart = FindStartOfDataPacket(lineStr, 13);
        printf("06b: %lu\n", messageStart);
    }

    return 1;
}

size_t FindStartOfDataPacket(MyString* str, size_t windowSize)
{
    for (size_t charIndex = windowSize; charIndex < str->Length; charIndex++)
    {
        if (WindowPriorCharsDiffer(str, charIndex, windowSize))
        {
            return charIndex + 1;
        }
    }

    assert(false);
}

bool WindowPriorCharsDiffer(MyString* str, size_t index, size_t windowSize)
{
    assert(index <= str->Length);
    assert(index >= windowSize);

    for (size_t startIndex = index-windowSize; startIndex <= index; startIndex++)
    {
        char currentChar = str->Characters[startIndex];
        for (size_t j = startIndex + 1; j <= index; j++)
        {
            if (startIndex == j) continue;

            if (str->Characters[j] == currentChar) return false;
        }
    }

    return true;
}