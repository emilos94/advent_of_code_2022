#include "arrlist_tests.h"

bool compareInts(void* either, void* other);

int arrlist_tests_run(void)
{
    _test_arrlist();
    _test_arrlist_swap();
    _test_arrlist_contains();
}

void _test_arrlist(void)
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Kilobytes(10));

    ArrayList* arrlist = arrlist_New(&arena, 10, sizeof(int));

    assert(arrlist->Capacity == 10);
    assert(arrlist->Count == 0);

    int item = 5;
    arrlist_Add(&arena, arrlist, &item);

    int* firstItem = arrlist_GetType(arrlist, int, 0);
    assert(arrlist->Count == 1);
    assert((*firstItem) == 5);

    int item2 = 4;
    arrlist_Add(&arena, arrlist, &item2);

    firstItem = arrlist_GetType(arrlist, int, 0);
    int* secondItem = arrlist_GetType(arrlist, int, 1);
    assert((*firstItem) == 5);
    assert((*secondItem) == 4);

    arrlist_Sort(arrlist, &compareInts);

    firstItem = arrlist_GetType(arrlist, int, 0);
    secondItem = arrlist_GetType(arrlist, int, 1);
    assert((*firstItem) == 4);
    assert((*secondItem) == 5);

    memory_arena_free(&arena);
}


void _test_arrlist_swap(void)
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Kilobytes(1));

    ArrayList* arrlist = arrlist_New(&arena, 10, sizeof(int));
    int a = 10;
    int b = 100;
    arrlist_Add(&arena, arrlist, &a);
    arrlist_Add(&arena, arrlist, &b);

    assert(*(arrlist_GetType(arrlist, int, 0)) == a);
    assert(*(arrlist_GetType(arrlist, int, 1)) == b);

    _arrlist_swap(arrlist, 0, 1);

    int da = *(arrlist_GetType(arrlist, int, 0));
    int db = *(arrlist_GetType(arrlist, int, 1));

    assert(da == b);
    assert(db == a);

    memory_arena_free(&arena);
}

void _test_arrlist_contains(void)
{
    memory_ArenaBlock(arena, Kilobytes(1))
    {
        ArrayList* arrlist = arrlist_New(&arena, 2, sizeof(int));
        int a = 10;
        int b = 100;
        arrlist_Add(&arena, arrlist, &a);
        arrlist_Add(&arena, arrlist, &b);

        int c = 10;
        int d = 20;
        assert(arrlist_Contains(arrlist, &c));
        assert(!arrlist_Contains(arrlist, &d));
    }
}

bool compareInts(void* either, void* other)
{
    int* eitherI = (int*)either;
    int* otherI = (int*)other;
    bool result = (*eitherI) < (*otherI);
    return result;
}