#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"
#include "../util/hashmap.h"

struct Operation
{
    MyString* Operation;
    int Value;
};
typedef struct Operation Operation;

Operation* ParseOperation(MemoryArena* arena, MyString* fileLine);
void AddToRegisterIfCycleMod40Is20(int* sum, int cycle, int cpuRegister);
void OutputPixel(MyString* screen, int crtWidth, int cycle, int cpuRegister);
void PrintScreen(MyString* screen);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(1));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../../data/10.txt", 10);

    int crtWidth = 40;
    int crtHeight = 6;
    MyString* screen = str_InitWithChar(&arena, '.', crtWidth * crtHeight);

    int sum = 0;
    int cycle = 0;
    int cpuRegister = 1;
    list_Loop(fileLines, MyString, lineNode, lineStr)
    {
        str_TrimEndWhiteSpace(lineStr);
        Operation* operation = ParseOperation(&arena, lineStr);

        bool isAdd = str_EqualsChars(operation->Operation, "addx");
        
        cycle++;
        OutputPixel(screen, crtWidth, cycle, cpuRegister);
        AddToRegisterIfCycleMod40Is20(&sum, cycle, cpuRegister);
        if (isAdd)
        {
            OutputPixel(screen, crtWidth, cycle, cpuRegister);
            cycle++;
            cpuRegister += operation->Value;
            AddToRegisterIfCycleMod40Is20(&sum, cycle, cpuRegister);
            OutputPixel(screen, crtWidth, cycle, cpuRegister);
        }
    }

    printf("10a: %d\n", sum);
    printf("10b:\n");
    PrintScreen(screen);
    memory_arena_free(&arena);
}

void PrintScreen(MyString* screen)
{
    size_t charIndex = 0;
    while (charIndex < screen->Length)
    {
        for (size_t rowStart = charIndex; rowStart < charIndex + 40; rowStart++)
        {
            printf("%c", screen->Characters[rowStart]);
        }
        printf("\n");
        charIndex += 40;
    }
}

void OutputPixel(MyString* screen, int crtWidth, int cycle, int cpuRegister)
{
    int row = cycle / crtWidth;
    int rowIndex = cycle % crtWidth;
    if (abs(rowIndex - cpuRegister) <= 1)
    {
        screen->Characters[row * crtWidth + rowIndex] = '#';
    }
    else 
    {
        screen->Characters[row * crtWidth + rowIndex] = '.';
    }
}

void AddToRegisterIfCycleMod40Is20(int* sum, int cycle, int cpuRegister)
{
    if (cycle % 40 == 19)
    {
        sum[0] += (cycle + 1) * cpuRegister;
    }
}

Operation* ParseOperation(MemoryArena* arena, MyString* fileLine)
{
    List* split = str_SplitByChar(fileLine, ' ', arena);

    Operation* op = memory_AllocateStruct(arena, Operation);

    MyString* operation = list_GetStruct(split, 0, MyString);
    op->Operation = operation;

    if (str_EqualsChars(operation, "addx")) 
    {
        int value;
        str_ToInt(list_GetStruct(split, 1, MyString), &value);
        op->Value = value;
    }

    return op;
}

