#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"
#include "../util/file_system.h"

ListNode* FindDirectoryNodeByName(List* directories, MyString* name);
bool IsCommandLine(MyString* line);
bool IsDirectoryLine(MyString* line);
void ResolveCommand(MemoryArena* arena, List* directoryQueue, List* allDirectories, List* line);

int main(void)
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(1));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../../data/07.txt", 100);
    List* directoryQueue = list_Init(&arena, sizeof(Directory));
    List* allDirectories = list_Init(&arena, sizeof(Directory));
    
    MyString* baseDirName = str_Init(&arena, "/\0");
    Directory* baseDirectory = dir_Init(&arena, baseDirName);
    list_PushBack(&arena, allDirectories, baseDirectory);
    list_Add(&arena, directoryQueue, baseDirectory, sizeof(Directory));

    list_Loop(fileLines, MyString, listNode, lineStr)
    {
        if (str_StartsWith(lineStr, "$ cd /", 6))
        {
            continue;
        }
        str_TrimEndWhiteSpace(lineStr);
        List* lineContent = str_SplitByChar(lineStr, ' ', &arena);

        if (IsCommandLine(lineStr))
        {
            ResolveCommand(&arena, directoryQueue, allDirectories, lineContent);
        }
        else if (IsDirectoryLine(lineStr))
        {
            Directory* subDir = dir_Init(&arena, list_GetStruct(lineContent, 1, MyString));
            Directory* currentDir = (Directory*)directoryQueue->Tail->Data;
            list_Add(&arena, currentDir->Directories, subDir, sizeof(Directory));
            list_Add(&arena, allDirectories, subDir, sizeof(Directory));
        }
        else // file
        {
            MyString* fileSize = list_GetStruct(lineContent, 0, MyString);
            MyString* fileName = list_GetStruct(lineContent, 1, MyString);
            Directory* currentDir = (Directory*)directoryQueue->Tail->Data;

            File* newFile = memory_AllocateStruct(&arena, File);
            newFile->name = fileName;
            str_ToSizeT(fileSize, &newFile->Size);
            list_Add(&arena, currentDir->Files, newFile, sizeof(File));
        }
    }

    size_t dirSizeSum = 0;
    size_t totalSystemSpaceUsed = 0;
    {
        list_Loop(allDirectories, Directory, dirNode, dir)
        {
            size_t size = dir_SizeOfDirectory(dir);
            if (size <= 100000)
            {
                dirSizeSum += size;
            }
            if (size > totalSystemSpaceUsed)
            {
                totalSystemSpaceUsed = size;
            }
        }
    }

    size_t systemSpace = 70000000;
    size_t updateSpaceRequired = 30000000;

    size_t availableSpace = systemSpace - totalSystemSpaceUsed;
    size_t needToDelete = updateSpaceRequired - availableSpace;

    size_t minimumToFreeRequired = 100000000000;

    list_Loop(allDirectories, Directory, dirNode, dir)
    {
        size_t size = dir_SizeOfDirectory(dir);
        bool freesUpEnoughSpace = size >= needToDelete;
        bool isSmallestEligibleDir = size < minimumToFreeRequired;

        if (freesUpEnoughSpace && isSmallestEligibleDir)
        {
            minimumToFreeRequired = size;
        }
    }

    printf("07a: %lu\n", dirSizeSum);
    printf("07b: %lu\n", minimumToFreeRequired);

    memory_arena_free(&arena);
    return 1;
}

ListNode* FindDirectoryNodeByName(List* directories, MyString* name)
{
    list_Loop(directories, Directory, node, dir)
    {
        if (str_Equals(dir->Name, name))
        {
            return node;
        }
    }

    return NULL;
}


bool IsCommandLine(MyString* line)
{
    return str_StartsWith(line, "$", 1);
}

bool IsDirectoryLine(MyString* line)
{
    return str_StartsWith(line, "dir", 3);
}

void ResolveCommand(MemoryArena* arena, List* directoryQueue, List* allDirectories, List* line)
{
    MyString* command = list_GetStruct(line, 1, MyString);

    if (str_StartsWith(command, "cd", 2))
    {
        MyString* argument = list_GetStruct(line, 2, MyString);
        if (str_StartsWith(argument, "..", 2))
        {
            list_PopBack(directoryQueue);
        }
        else 
        {
            Directory* currentDirectory = list_PeekBackValue(directoryQueue, Directory);
            ListNode* existing = FindDirectoryNodeByName(currentDirectory->Directories, argument);
            bool directoryExists = existing != NULL;
            assert(directoryExists);
            list_Add(arena, directoryQueue, existing->Data, sizeof(Directory));
        }
    }
}