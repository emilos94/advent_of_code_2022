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
        ListNode* oldTail = list->Tail;
        listNode->Prev = oldTail;
        oldTail->Next = listNode;
        list->Tail = listNode;
    }

    list->Size++;
}

void* list_Get(List* list, size_t index)
{
    assert(index <= list->Size-1);

    size_t currentIndex = 0;
    ListNode* currentNode = list->Head;
    while (currentIndex != index)
    {
        currentNode = currentNode->Next;
        currentIndex++;
    }

    return currentNode->Data;
}


ListNode* list_PopBack(List* list)
{
    assert(list->Size > 0);

    ListNode* tail = list->Tail;
    
    if (list->Size == 1) 
    {
        list->Head = NULL;
        list->Tail = NULL;
    }
    else 
    {
        ListNode* newTail = tail->Prev;
        newTail->Next = NULL;
        list->Tail = newTail;
    }

    list->Size--;

    tail->Next = NULL;
    tail->Prev = NULL;
    return tail;
}


ListNode* list_PopFront(List* list)
{
    assert(list->Size > 0);

    ListNode* head = list->Head;

    if (list->Size == 1) 
    {
        list->Head = NULL;
        list->Tail = NULL;
    }
    else
    {
        ListNode* newHead = head->Next;
        newHead->Prev = NULL;
        list->Head = newHead;
    }

    list->Size--;
    head->Next = NULL;
    head->Prev = NULL;
    return head;
}


ListNode* list_PeekBack(List* list)
{
    return list->Tail;
}


ListNode* list_PeekFront(List* list)
{
    return list->Head;
}

void list_PushNodeBack(List* list, ListNode* node)
{
    if (list->Size == 0)
    {
        list->Head = node;
        list->Tail = node;
    }
    else 
    {
        ListNode* currentTail = list->Tail;
        currentTail->Next = node;
        node->Prev = currentTail;
        list->Tail = node;
    }
    
    list->Size++;
}

void list_PushNodeFront(List* list, ListNode* node)
{
    if (list->Size == 0)
    {
        list->Head = node;
        list->Tail = node;
    }
    else 
    {
        ListNode* currentHead = list->Head;
        currentHead->Prev = node;
        node->Next = currentHead;
        list->Head = node;
    }

    list->Size++;
}

ListNode* listNode_Copy(MemoryArena* arena, ListNode* node, size_t sizeOfData)
{
    ListNode* copy = memory_AllocateStruct(arena, ListNode);
    copy->Next = node->Next;
    copy->Prev = node->Prev;
    copy->Data = memory_allocate(arena, sizeOfData);
    memcpy(copy->Data, node->Data, sizeOfData);
    return copy;
}


void* list_AddEmpty(MemoryArena* arena, List* list, size_t size)
{
    ListNode* node = memory_AllocateStruct(arena, ListNode);

    if (list->Size == 0) 
    {
        list->Head = node;
        list->Tail = node;
    }
    else 
    {
        ListNode* currentTail = list->Tail;
        currentTail->Next = node;
        node->Prev = currentTail;
        list->Tail = node;
    }

    node->Data = memory_allocate(arena, size);
    list->Size++;
    return node->Data;
}

ListNode* list_PushBack(MemoryArena* arena, List* list, void* data)
{
    ListNode* node = memory_AllocateStruct(arena, ListNode);
    node->Data = data;
    if (list->Size == 0) 
    {
        list->Head = node;
        list->Tail = node;
    }
    else 
    {
        ListNode* currentTail = list->Tail;
        currentTail->Next = node;
        node->Prev = currentTail;
        list->Tail = node;
    }

    list->Size++;
    return node->Data;
}

// First predicate param is map entry, second is the param here used for filtering
void* list_FindByPredicate(List* list, bool (*predicate)(void*, void*), void* param)
{
    list_Loop(list, void, node, val)
    {
        if ((*predicate)(val, param))
        {
            return val;
        }
    }

    return NULL;
}
