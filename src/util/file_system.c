#include "file_system.h"


Directory* dir_Init(MemoryArena* arena, MyString* name)
{
    Directory* dir = memory_AllocateStruct(arena, Directory);
    dir->Name = name;
    dir->Directories = list_Init(arena, sizeof(Directory));
    dir->Files = list_Init(arena, sizeof(File));
    return dir;
}

size_t dir_SizeOfDirectory(Directory* dir)
{
    assert(dir != NULL);
    
    size_t size = 0;

    list_Loop(dir->Directories, Directory, node, subDir)
    {
        size += dir_SizeOfDirectory(subDir);
    }

    list_Loop(dir->Files, File, fileNode, file)
    {
        size += file->Size;
    }

    return size;
}

void dir_PrettyPrint(Directory* dir)
{
    assert(dir != NULL);

    _PrettyPrint(dir, 0);
}


void _PrettyPrint(Directory* dir, size_t depth)
{
    assert(dir != NULL);

    _PrintSpaces(depth);
    printf("(d) ");
    str_Print(dir->Name);
    printf("\n");

    list_Loop(dir->Files, File, fileNode, file)
    {
        _PrintSpaces(depth + 2);
        printf("(f) ");
        str_Print(file->name);
        printf(" %lu\n", file->Size);
    }

    list_Loop(dir->Directories, Directory, dirNode, subDir)
    {
        _PrintSpaces(depth);
        _PrettyPrint(subDir, depth + 1);
    }
}

void _PrintSpaces(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf(" ");
    }
}