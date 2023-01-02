#include "arraylist.h"

ArrayList* arrlist_New(MemoryArena* arena, size_t capacity, size_t sizeOfElement)
{
    ArrayList* arrlist = memory_AllocateStruct(arena, ArrayList);
    arrlist->Data = memory_allocate(arena, capacity * sizeOfElement);
    arrlist->Capacity = capacity;
    arrlist->Count = 0;
    arrlist->SizePerElement = sizeOfElement;
    return arrlist;
}


ArrayList* arrlist_Copy(MemoryArena* arena, ArrayList* toCopy)
{
    ArrayList* copy = arrlist_New(arena, toCopy->Capacity, toCopy->SizePerElement);
    copy->Count = toCopy->Count;
    memcpy(copy->Data, toCopy->Data, toCopy->Count * toCopy->SizePerElement);
    return copy;
}

void* arrlist_Get(ArrayList* list, size_t index)
{
    assert(list && index < list->Capacity);

    return list->Data + index * list->SizePerElement;
}

void arrlist_Set(ArrayList* list, size_t index, void* data)
{
    assert(list && index < list->Capacity);

    void* dataPtr = list->Data + index * list->SizePerElement;
    memcpy(dataPtr, data, list->SizePerElement);
}

void arrlist_Add(MemoryArena* arena, ArrayList* list, void* item)
{
    assert(arena && list);

    if (list->Count == list->Capacity - 1)
    {
        _arrlist_Resize(arena, list);
    }

    void* dataPtr = list->Data + list->Count * list->SizePerElement;
    memcpy(dataPtr, item, list->SizePerElement);
    list->Count++;
}

void arrlist_Remove(ArrayList* list, size_t index)
{
    assert(list && index < list->Count && index < list->Capacity);
    
    if (index == list->Count - 1)
    {
        list->Count--;
        return;
    }

    size_t offset = index * list->SizePerElement;
    size_t countToMove = (list->Count - 1) * list->SizePerElement - offset;
    memcpy(list->Data + offset, list->Data + offset + 1, countToMove);
}

void arrlist_Sort(ArrayList* list, bool (*comparisonFunc)(void*, void*))
{
    _arrlist_quickSort(list, 0, list->Count-1, comparisonFunc);
}

bool arrlist_Contains(ArrayList* list, void* data)
{
    char* dataChar = (char*) data;
    for (size_t index = 0; index < list->Count; index++)
    {
        if (index == list->Count) 
        {
            int x = 0;
        }
        char* currentData = (char*) arrlist_Get(list, index);
        bool isSame = true;
        for (size_t byteIndex = 0; byteIndex < list->SizePerElement; byteIndex++)
        {
            if (dataChar[byteIndex] != currentData[byteIndex])
            {
                isSame = false;
                break;
            }
        }

        if (isSame)
        {
            return true;
        }
    }

    return false;
}

void _arrlist_Resize(MemoryArena* arena, ArrayList* list)
{
    size_t newCapacity = list->Capacity + list->Capacity / 2;

    void* newData = memory_allocate(arena, newCapacity * list->SizePerElement);
    memcpy(newData, list->Data, list->Count * list->SizePerElement);
    list->Data = newData;
}

void _arrlist_quickSort(ArrayList* list, int low, int high, bool (*comparisonFunc)(void*, void*))
{
    if (low >= high || low < 0)
        return;

    int pIndex = _arrlist_partition(list, low, high, comparisonFunc);

    _arrlist_quickSort(list, low, pIndex-1, comparisonFunc);
    _arrlist_quickSort(list, pIndex+1, high, comparisonFunc);
}

int _arrlist_partition(ArrayList* list, int low, int high, bool (*comparisonFunc)(void*, void*))
{
    void* pivot = arrlist_Get(list, high);

    int i = low - 1;

    for(int j = low; j <= high-1; j++)
    {
        void* other = arrlist_Get(list, j);
        if ((*comparisonFunc)(other, pivot)) 
        {
            i++;
            _arrlist_swap(list, i, j);
        }
    }

    i++;
    _arrlist_swap(list, i, high);
    return i;
}

void _arrlist_swap(ArrayList* list, int a, int b) 
{
    assert(a >= 0 && b >= 0);

    unsigned char* either = arrlist_Get(list, a);
    unsigned char* other = arrlist_Get(list, b);
    unsigned char tmp;
    for (size_t i = 0; i != list->SizePerElement; ++i)
    {
        tmp = either[i];
        either[i] = other[i];
        other[i] = tmp;
    }
    //void* tempEither = arrlist_Get(list, i);
    //void* tempOther = arrlist_Get(list, j);
    //arrlist_Set(list, i, tempOther);
    //arrlist_Set(list, j, tempEither);
}
