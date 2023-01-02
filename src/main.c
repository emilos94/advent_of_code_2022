#include "util/file_util.h"
#include "util/memory.h"

#include "12.h"


int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(5));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../data/12.txt", 100);

    TwelveSolve(&arena, fileLines);
    return 1;
}
