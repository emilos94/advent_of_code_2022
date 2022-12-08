#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"

struct MoveCommand
{
    int StackFrom;
    int StackTo;
    int Count;
};
typedef struct MoveCommand MoveCommand;

size_t GetStackCount(MyString* line);
void InitStacks(MemoryArena* arena, List* stacks, size_t stackCount);
void AddCratesToStacks(MemoryArena* arena, List* stacks, MyString* line);
MoveCommand ParseMoveCommandFromLine(MemoryArena* arena, MyString* line);
void ApplyMoveCommand(List* stacks, MoveCommand* command);
void ApplyMoveCommandCrane9001(MemoryArena* arena, List* stacks, MoveCommand* command);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(10));

    List* fileLines = list_Init(&arena, sizeof(MyString));
    file_read_lines(&arena, fileLines, "../../data/05.txt");

    List* stacks = list_Init(&arena, sizeof(List));
    List* stacksCrane9001 = list_Init(&arena, sizeof(List));

    ListNode* listNode = fileLines->Head;
    while (listNode != NULL)
    {
        MyString* line = str_Init(&arena, (char*)listNode->Data);
        size_t stackCount = GetStackCount(line);
        if (stacks->Size == 0) 
        {
            InitStacks(&arena, stacks, stackCount);
            InitStacks(&arena, stacksCrane9001, stackCount);
        }
        if (str_ContainsChar(line, '['))
        {
            AddCratesToStacks(&arena, stacks, line);
            AddCratesToStacks(&arena, stacksCrane9001, line);
        }
        if (str_StartsWith(line, "move", 4))
        {
            str_TrimEndWhiteSpace(line);
            MoveCommand command = ParseMoveCommandFromLine(&arena, line);
            ApplyMoveCommand(stacks, &command);
            ApplyMoveCommandCrane9001(&arena, stacksCrane9001, &command);
        }

        listNode = listNode->Next;
    }

    printf("05a: ");
    list_Loop(stacks, List, stackListNode, stack)
    {
        char* topCrate = list_PeekFrontValue(stack, char);
        printf("%c", topCrate[0]);
    }
    printf("\n05b: ");
    list_Loop(stacksCrane9001, List, stackListNode2, stackCrane9001)
    {
        char* topCrate = list_PeekFrontValue(stackCrane9001, char);
        printf("%c", topCrate[0]);
    }
}


size_t GetStackCount(MyString* line)
{
    // Each stack char '[A] ' is four chars, except the last
    return (line->Length+1) / 4;
}


void InitStacks(MemoryArena* arena, List* stacks, size_t stackCount)
{
    for (size_t i = 0; i < stackCount; i++)
    {
        List* stack = list_Init(arena, sizeof(char));
        list_Add(arena, stacks, stack, sizeof(List));
    }
}


void AddCratesToStacks(MemoryArena* arena, List* stacks, MyString* line)
{
    size_t crateIndex = 1;
    list_Loop(stacks, List, listNode, stack)
    {
        char currentChar = line->Characters[crateIndex];
        if (currentChar != ' ')
        {
            list_Add(arena, stack, &currentChar, sizeof(char));
        }
        crateIndex += 4;
    }
}


MoveCommand ParseMoveCommandFromLine(MemoryArena* arena, MyString* line)
{
    List* splitResult = str_SplitByChar(line, ' ', arena);
    
    assert(splitResult->Size == 6);

    MyString* countStr = (MyString*)list_Get(splitResult, 1);
    MyString* stackFromStr = (MyString*)list_Get(splitResult, 3);
    MyString* stackToStr = (MyString*)list_Get(splitResult, 5);

    MoveCommand command;
    str_ToInt(stackFromStr, &command.StackFrom);
    str_ToInt(stackToStr, &command.StackTo);
    str_ToInt(countStr, &command.Count);

    return command;
}


void ApplyMoveCommand(List* stacks, MoveCommand* command)
{
    List* stackFrom = (List*)list_Get(stacks, command->StackFrom-1);
    List* stackTo = (List*)list_Get(stacks, command->StackTo-1);

    size_t moveCount = 0;
    while (moveCount++ < command->Count)
    {
        char* front = list_PeekFrontValue(stackFrom, char);
        ListNode* node = list_PopFront(stackFrom);
        list_PushNodeFront(stackTo, node);
    }
}

void ApplyMoveCommandCrane9001(MemoryArena* arena, List* stacks, MoveCommand* command)
{
    List* stackFrom = (List*)list_Get(stacks, command->StackFrom-1);
    List* stackTo = (List*)list_Get(stacks, command->StackTo-1);

    List* tempMoveList = list_Init(arena, sizeof(char));
    size_t moveCount = 0;
    while (moveCount++ < command->Count)
    {
        ListNode* node = list_PopFront(stackFrom);
        list_PushNodeFront(tempMoveList, node);
    }

    list_Loop(tempMoveList, char, tempNode, crate)
    {
        ListNode* copy = listNode_Copy(arena, tempNode, tempMoveList->SizePerEntry);
        list_PushNodeFront(stackTo, copy);
    }
}
