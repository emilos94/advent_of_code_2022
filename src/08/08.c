#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"

struct Grid
{
    size_t* Trees;
    size_t GridWidth;
    size_t GridHeight;
};
typedef struct Grid Grid;

Grid* ParseGrid(MemoryArena* arena, List* fileLines);
size_t GetTreeHeight(Grid* grid, size_t x, size_t y);
bool IsVisible(Grid* grid, size_t x, size_t y);
size_t GetScenicScore(Grid* grid, size_t x, size_t y);

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(1));

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../../data/08.txt", 1000);
    list_Loop(fileLines, MyString, node, line)
    {
        str_TrimEndWhiteSpace(line);
    }

    Grid* grid = ParseGrid(&arena, fileLines);

    size_t maxScenicScore = 0;
    size_t visibleCount = grid->GridWidth * 2 + grid->GridHeight * 2 - 4;
    for (size_t y = 1; y < grid->GridHeight-1; y++)
    {
        for (size_t x = 1; x < grid->GridWidth-1; x++)
        {
            if (IsVisible(grid, x, y))
            {
                visibleCount++;
            }
            size_t scenicScore = GetScenicScore(grid, x, y);
            if (scenicScore > maxScenicScore)
            {
                maxScenicScore = scenicScore;
            }
        }
    }

    printf("08a: %lu\n", visibleCount);
    printf("08b: %lu\n", maxScenicScore);

    memory_arena_free(&arena);
    return 1;
}


Grid* ParseGrid(MemoryArena* arena, List* fileLines)
{
    Grid* grid = memory_AllocateStruct(arena, Grid);
    grid->GridWidth = (list_PeekFrontValue(fileLines, MyString))->Length;
    grid->GridHeight = fileLines->Size;
    grid->Trees = memory_AllocateArray(arena, size_t, grid->GridHeight * grid->GridWidth);

    for (size_t y = 0; y < grid->GridHeight; y++)
    {
        MyString* line = list_GetStruct(fileLines, y, MyString);
        for (size_t x = 0; x < grid->GridWidth; x++)
        {
            size_t index = y * grid->GridWidth + x;
            char currentChar = line->Characters[x];
            size_t height = (size_t)((int)line->Characters[x] - '0');
            grid->Trees[index] = height;
        }
    }

    return grid;
}

size_t GetTreeHeight(Grid* grid, size_t x, size_t y)
{
    return grid->Trees[y * grid->GridWidth + x];
}

bool IsVisible(Grid* grid, size_t x, size_t y)
{
    bool isOnEdge = x == 0 || x == grid->GridWidth-1 || y == 0 || y == grid->GridHeight-1;
    if (isOnEdge)
    {
        return true;
    }

    size_t currentTreeHeight = GetTreeHeight(grid, x, y);
    
    int dy = y - 1;
    while (GetTreeHeight(grid, x, dy) < currentTreeHeight && dy-- > 0) {}
    bool isVisibleFromTop = dy == -1;
    if (isVisibleFromTop) return true;

    dy = y + 1;
    while (GetTreeHeight(grid, x, dy) < currentTreeHeight && dy++ < grid->GridHeight){}
    bool isVisibleFromBottom = dy >= grid->GridHeight;
    if (isVisibleFromBottom) return true;

    int dx = x - 1;
    while (GetTreeHeight(grid, dx, y) < currentTreeHeight && dx-- > 0) {}
    bool isVisibleFromLeft = dx == -1;
    if (isVisibleFromLeft) return true;

    dx = x + 1;
    while (GetTreeHeight(grid, dx, y) < currentTreeHeight && dx++ < grid->GridWidth) {}
    bool isVisibleFromRight = dx >= grid->GridWidth;
    if (isVisibleFromRight) return true;

    return false;
}

size_t GetScenicScore(Grid* grid, size_t x, size_t y)
{
    bool isOnEdge = x == 0 || x == grid->GridWidth-1 || y == 0 || y == grid->GridHeight-1;
    if (isOnEdge)
    {
        return 0;
    }

    size_t currentTreeHeight = GetTreeHeight(grid, x, y);
    
    int dy = y - 1;
    size_t topScenicScore = 0;
    for (dy = y-1; dy >= 0; dy--)
    {
        topScenicScore++;
        bool atEdge = dy == 0;
        if (GetTreeHeight(grid, x, dy) >= currentTreeHeight || atEdge) 
            break;
    }
    
    size_t botScenicScore = 0;
    for (dy = y+1; dy <= grid->GridHeight; dy++)
    {
        botScenicScore++;
        bool atEdge = dy == grid->GridHeight-1;
        if (GetTreeHeight(grid, x, dy) >= currentTreeHeight || atEdge) 
            break;
    }
    
    int dx = x - 1;
    size_t leftScenicScore = 0;
    for (dx = x - 1; dx >= 0; dx--)
    {
        leftScenicScore++;
        bool atEdge = dx == 0;
        if (GetTreeHeight(grid, dx, y) >= currentTreeHeight || atEdge) 
            break;
    }
    
    size_t rightScenicScore = 0;
    for (dx = x + 1; dx <= grid->GridWidth; dx++)
    {
        rightScenicScore++;
        bool atEdge = dx == grid->GridWidth-1;
        if (GetTreeHeight(grid, dx, y) >= currentTreeHeight || atEdge) 
            break;
    }
    
    return leftScenicScore * rightScenicScore * topScenicScore * botScenicScore;
}
