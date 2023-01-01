#ifndef LIST_H
#define LIST_H

#include "memory.h"
#include <string.h> // memcpy
#include <assert.h>
#include <stdbool.h>

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
List* list_Copy(MemoryArena* arena, List* toCopy);
void list_Add(MemoryArena* arena, List* list, void* data, size_t sizeOfData);
void* list_Get(List* list, size_t index);
void* list_AddEmpty(MemoryArena* arena, List* list, size_t size);

ListNode* listNode_Copy(MemoryArena* arena, ListNode* node, size_t sizeOfData);

ListNode* list_PopBack(List* list);
ListNode* list_PopFront(List* list);
void list_PushNodeBack(List* list, ListNode* node);
void list_PushNodeFront(List* list, ListNode* node);
ListNode* list_PeekBack(List* list);
ListNode* list_PeekFront(List* list);
ListNode* list_PushBack(MemoryArena* arena, List* list, void* data);

void* list_FindByPredicate(List* list, bool (*predicate)(void*, void*), void* param);

#define list_AddStruct(arena, list, type) (type*)list_Add(arena, list, sizeof(type))
#define list_GetStruct(list, index, type) (type*)list_Get(list, index)
#define list_PeekBackValue(list, type) (type*)list_PeekBack(list)->Data
#define list_PeekFrontValue(list, type) (type*)list_PeekFront(list)->Data
#define list_PopBackValue(list, type) (type*)list_PopBack(list)->Data
#define list_PopFrontValue(list, type) (type*)list_PopFront(list)->Data
#define list_Loop(list, type, nodeName, valueName) \
    ListNode* nodeName = NULL;\
    type* valueName = NULL;\
    for(nodeName = list->Size == 0 ? NULL : list->Head, valueName = nodeName == NULL ? NULL : (type*)nodeName->Data;\
        nodeName != NULL;\
        nodeName = nodeName->Next, valueName = nodeName == NULL ? NULL : (type*)nodeName->Data)

#define list_AddEmptyStruct(arena, list, type) (type*)list_AddEmpty(arena, list, sizeof(type))

#endif