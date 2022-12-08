#include "../util/my_string.h"
#include "../util/memory.h"
#include <assert.h>

void test_SplitByChar_EndsWithSplit(MemoryArena* arena);
void test_SplitByChar(MemoryArena* arena);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Kilobytes(1000));

    test_SplitByChar_EndsWithSplit(&arena);
    memory_arena_reset(&arena);

    test_SplitByChar(&arena);
    memory_arena_reset(&arena);

    

}


void test_SplitByChar_EndsWithSplit(MemoryArena* arena)
{
    MyString* str1 = str_Init(arena, "1 2 3 ");
    List* splitResult = str_SplitByChar(str1, ' ', arena);

    assert(splitResult->Size == 3);
    
    MyString* first = (MyString*)list_Get(splitResult, 0);
    assert(first->Length == 1);
    assert(first->Characters[0] == '1');

    MyString* second = (MyString*)list_Get(splitResult, 1);
    assert(second->Length == 1);
    assert(second->Characters[0] == '2');

    MyString* third = (MyString*)list_Get(splitResult, 2);
    assert(third->Length == 1);
    assert(third->Characters[0] == '3');
}

void test_SplitByChar(MemoryArena* arena)
{
    MyString* testStr = str_Init(arena, "move 1 to 2 from 3");

    List* splitResult = str_SplitByChar(testStr, ' ', arena);

    assert(splitResult->Size == 6);
    
}