#include "./memory.h"

void memory_arena_initialize(MemoryArena* arena, size_t count)
{
    arena->Memory = calloc(1, count);
    arena->Offset = 0;
    arena->Capacity = count;
}

void memory_arena_free(MemoryArena* arena)
{
    free(arena->Memory);
}


void memory_arena_reset(MemoryArena* arena)
{
    arena->Offset = 0;
}

void* memory_allocate(MemoryArena* arena, size_t count)
{
    assert (arena->Offset + count <= arena->Capacity);

    void* currentPointer = arena->Memory + arena->Offset;
    arena->Offset += count;
    return currentPointer;
}
