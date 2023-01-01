#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "memory.h"
#include <assert.h>
#include <string.h> // memcpy
#include <stdbool.h>

struct ArrayList
{
    void* Data;
    size_t Capacity;
    size_t Count;
    size_t SizePerElement;
};
typedef struct ArrayList ArrayList;

ArrayList* arrlist_New(MemoryArena* arena, size_t capacity, size_t sizeOfElement);
ArrayList* arrlist_Copy(MemoryArena* arena, ArrayList* toCopy);

void* arrlist_Get(ArrayList* list, size_t index);
void arrlist_Add(MemoryArena* arena, ArrayList* list, void* item);
void arrlist_Remove(ArrayList* list, size_t index);
void arrlist_Sort(ArrayList* list, bool (*comparisonFunc)(void*, void*));

void _arrlist_Resize(MemoryArena* arena, ArrayList* list);

void _arrlist_quickSort(ArrayList* list, int low, int high, bool (*comparisonFunc)(void*, void*));
int _arrlist_partition(ArrayList* list, int low, int high, bool (*comparisonFunc)(void*, void*));
void _arrlist_swap(ArrayList* list, int i, int j);

#define arrlist_GetType(arraylist, type, index) (type*)arrlist_Get(arraylist, index)
#define arrlist_Foreach(arraylist, type, varname) \
    size_t index = 0; \
    type* varname = NULL; \
    for(index = 0, varname = arraylist->Count > 0 ? arrlist_GetType(arraylist, type, 0) : NULL; \
        index < arraylist->Count; \ 
        index++, varname = arrlist_GetType(arraylist, type, index)) \

#endif