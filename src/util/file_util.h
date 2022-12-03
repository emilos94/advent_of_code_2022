#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <stdlib.h>

#include "./list.h"

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

#endif