#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "../util/list.h"
#include "../util/my_string.h"
#include "../util/memory.h"

#include <assert.h>

typedef struct Directory Directory;
struct Directory
{
    MyString* Name;
    List* Files;
    List* Directories;
};

struct File
{
    MyString* name;
    size_t Size;
};
typedef struct File File;

Directory* dir_Init(MemoryArena* arena, MyString* name);
size_t dir_SizeOfDirectory(Directory* dir);
void dir_PrettyPrint(Directory* dir);


void _PrettyPrint(Directory* dir, size_t size);
void _PrintSpaces(size_t count);

#endif
