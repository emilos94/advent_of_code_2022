#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "./my_string.h"
#include "./memory.h"
#include "./list.h"

struct MapEntry
{
    MyString* Key;
    void* Data;
};
typedef struct MapEntry MapEntry;

struct MapNode
{
    List* MapEntries;
};
typedef struct MapNode MapNode;

struct HashMap
{
    MapNode* MapNodes;
    size_t NodeCount;
    size_t Count;
};
typedef struct HashMap HashMap;

HashMap* map_NewMap(MemoryArena* arena, size_t nodeCount);
void map_Insert(MemoryArena* arena, HashMap* map, MyString* key, void* Data, size_t sizeOfData);
void* map_Get(HashMap* map, MyString* key);
bool map_Contains(HashMap* map, MyString* key);
void map_PrettyPrint(HashMap* map, void (*dataPrint)(void*));
List* map_GetKeys(MemoryArena* arena, HashMap* map);

int _map_hash(MyString* str);
bool _map_entry_key_equals(void* mapEntryPtr, void* keyPtr);

#endif
