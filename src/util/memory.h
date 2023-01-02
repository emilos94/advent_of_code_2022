#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <assert.h>
#include <string.h> // memcpy

struct MemoryArena
{
    void* Memory;
    size_t Offset;
    size_t Capacity;
};
typedef struct MemoryArena MemoryArena;

#define Kilobytes(x) x * 1024
#define Megabytes(x) Kilobytes(x) * 1024

void memory_arena_initialize(MemoryArena* arena, size_t count);
void memory_arena_free(MemoryArena* arena);
void memory_arena_reset(MemoryArena* arena);
void* memory_allocate(MemoryArena* arena, size_t count);
void* memory_allocate_copy(MemoryArena* arena, void* data, size_t size);

#define memory_AllocateArray(arena, type, count) (type*)memory_allocate(arena, sizeof(type) * count)
#define memory_AllocateStruct(arena, type) (type*)memory_allocate(arena, sizeof(type))
#define memory_AllocateCopy(arena, type, data) (type*)memory_allocate_copy(arena, data, sizeof(type))

#define memory_ArenaBlock(name, size) \
    MemoryArena name; \
    memory_arena_initialize(&name, size); \
    for (size_t index = 0; index < 1; index++, memory_arena_free(&name)) \

#endif