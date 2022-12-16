#include "./hashmap.h"

HashMap* map_NewMap(MemoryArena* arena, size_t nodeCount)
{
    HashMap* map = memory_AllocateStruct(arena, HashMap);
    map->NodeCount = nodeCount;
    map->MapNodes = memory_AllocateArray(arena, MapNode, nodeCount);

    for (size_t index = 0; index < nodeCount; index++)
    {
        map->MapNodes[index].MapEntries = list_Init(arena, sizeof(MapEntry));
    }

    return map;
}

void map_Insert(MemoryArena* arena, HashMap* map, MyString* key, void* data, size_t sizeOfData)
{
    int hash = _map_hash(key);
    int index = hash % map->NodeCount;
    MapNode mapNode = map->MapNodes[index];

    MapEntry* existingEntry = list_FindByPredicate(mapNode.MapEntries, &_map_entry_key_equals, key);
    bool containsKey = existingEntry != NULL;
    if (containsKey)
    {
        existingEntry->Data = memory_allocate_copy(arena, data, sizeOfData);
    }
    else 
    {
        MapEntry* entry = memory_AllocateStruct(arena, MapEntry);
        entry->Key = str_InitLimit(arena, key->Characters, key->Length);
        entry->Data = memory_allocate_copy(arena, data, sizeOfData);

        list_Add(arena, mapNode.MapEntries, entry, sizeof(MapEntry));
        map->Count++;
    }
}

void* map_Get(HashMap* map, MyString* key)
{
    int hash = _map_hash(key);
    int index = hash % map->NodeCount;
    MapNode mapNode = map->MapNodes[index];

    list_Loop(mapNode.MapEntries, MapEntry, node, entry)
    {
        if (str_Equals(key, entry->Key))
        {
            return entry->Data;
        }
    }

    assert(false);
}

bool map_Contains(HashMap* map, MyString* key)
{
    int hash = _map_hash(key);
    int index = hash % map->NodeCount;
    MapNode mapNode = map->MapNodes[index];

    list_Loop(mapNode.MapEntries, MapEntry, node, entry)
    {
        if (str_Equals(key, entry->Key))
        {
            return true;
        }
    }

    return false;
}


List* map_GetKeys(MemoryArena* arena, HashMap* map)
{
    List* keys = list_Init(arena, sizeof(MyString));

    for (size_t index = 0; index < map->NodeCount; index++)
    {
        MapNode node = map->MapNodes[index];
        if (node.MapEntries->Size > 0) 
        {
            list_Loop(node.MapEntries, MapEntry, entryNode, entry)
            {
                list_Add(arena, keys, entry->Key, sizeof(MyString));
            }
        }
    }

    return keys;
}

void map_PrettyPrint(HashMap* map, void (*dataPrint)(void*))
{
    for (size_t index = 0; index < map->NodeCount; index++)
    {
        MapNode node = map->MapNodes[index];
        if (node.MapEntries->Size > 0) 
        {
            list_Loop(node.MapEntries, MapEntry, entryNode, entry)
            {
                str_Print(entry->Key);
                printf(" -> ");
                (*dataPrint)(entry->Data);
                printf("\n");
            }
        }
    }
}

int _map_hash(MyString* str)
{
    unsigned long hash = 5381;

    for (size_t index = 0; index < str->Length; index++)
    {
        hash = ((hash << 5) + hash) + (int)str->Characters[index];
    }

    return hash;
}

bool _map_entry_key_equals(void* mapEntryPtr, void* keyPtr)
{
    MapEntry* entry = (MapEntry*)mapEntryPtr;
    MyString* key = (MyString*)keyPtr;

    return str_Equals(entry->Key, key);
}
