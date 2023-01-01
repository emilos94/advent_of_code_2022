#include "util/file_util.h"
#include "util/memory.h"

#include "11.h"


int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(10));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../data/11-test.txt", 50);

    return eleven_solve(&arena, fileLines);
}
