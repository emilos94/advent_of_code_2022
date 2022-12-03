#ifndef LIST_H
#define LIST_H

#include "memory.h"
#include <string.h> // memcpy
#include <assert.h>

typedef struct ListNode ListNode;
struct ListNode
{
    ListNode* Next;
    ListNode* Prev;
    void* Data;
};

struct List
{
    ListNode* Head;
    ListNode* Tail;
    size_t Size;
    size_t SizePerEntry;
};
typedef struct List List;

List* list_Init(MemoryArena* arena, size_t sizePerEntry);
void list_Add(MemoryArena* arena, List* list, void* data, size_t sizeOfData);
void* list_Get(List* list, size_t index);

#define list_AddStruct(arena, list, type) (type*)list_Add(arena, list, sizeof(type))
#define list_GetStruct(list, index, type) (type*)list_Get(list, index);

#endif