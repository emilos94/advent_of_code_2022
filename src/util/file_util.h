#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdlib.h>

#include "./list.h"
#include "./my_string.h"

#define MAX_LINE_LENGTH 1000
void file_read_lines(MemoryArena* arena, List* resultList, char* path)
{
    FILE *textfile;
    char line[MAX_LINE_LENGTH];
     
    textfile = fopen(path, "r");
    if(textfile == NULL)
    {
        return;
    }
    
    size_t lineIndex = 0;
    while(fgets(line, MAX_LINE_LENGTH, textfile))
    {
        list_Add(arena, resultList, &line, sizeof(line));
    }
    fclose(textfile);
}

List* file_read_lines_as_mystrings_max_line_length(MemoryArena* arena, char* path, size_t lineLength)
{
    FILE *textfile;
    char line[lineLength];
     
    List* resultList = list_Init(arena, sizeof(line));
    textfile = fopen(path, "r");
    if(textfile == NULL)
    {
        printf("Failed to read file: %s", path);
        return resultList;
    }
    size_t lineIndex = 0;
    while(fgets(line, lineLength, textfile))
    {
        MyString* str = str_Init(arena, line);
        list_Add(arena, resultList, str, sizeof(MyString));
    }
    fclose(textfile);
    return resultList;
}

#endif