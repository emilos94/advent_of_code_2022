#include "./list.h"

List* list_Init(MemoryArena* memoryArena, size_t sizePerEntry)
{
    List* list = memory_AllocateStruct(memoryArena, List);
    list->Size = 0;
    list->SizePerEntry = sizePerEntry;
    return list;
}

void list_Add(MemoryArena* arena, List* list, void* data, size_t sizeOfData)
{
    ListNode* listNode = memory_AllocateStruct(arena, ListNode);
    listNode->Data = memory_allocate(arena, sizeOfData);
    listNode->Next = NULL;
    memcpy(listNode->Data, data, sizeOfData);

    if (list->Size == 0)
    {
        list->Head = listNode;
        list->Tail = listNode;
    }
    else
    {
        ListNode* currentTail = list->Tail;
        list->Tail = listNode;
        listNode->Prev = currentTail;
        currentTail->Next = listNode;
    }

    list->Size = list->Size + 1;
}

void* list_Get(List* list, size_t index)
{
    assert(index <= list->Size-1);

    size_t currentIndex = 0;
    ListNode* currentNode = list->Head;
    while (currentIndex <= index)
    {
        currentNode = currentNode->Next;
    }

    return currentNode->Data;
}
