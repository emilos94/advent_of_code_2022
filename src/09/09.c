#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"
#include "../util/my_string.h"
#include "../util/hashmap.h"

struct Motion
{
    char Direction;
    size_t Count;
};
typedef struct Motion Motion;

struct Position
{
    int X;
    int Y;
};
typedef struct Position Position;

Motion ParseMotion(MemoryArena* arena, MyString* fileLine);
MyString* PositionToString(MemoryArena* arena, Position* position);
void ApplyMotionOneStep(Motion* motion, Position* headPosition);
bool ApplyTailFollow(Position* oldHeadPos, Position* currentHeadPos, Position* tailPosition);
void PrintRopeSim(MemoryArena* arena, HashMap* map);
List* InitTenPositionsList(MemoryArena* arena);

int main(void)
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(2));

    MemoryArena arenaTmpStrings;
    memory_arena_initialize(&arenaTmpStrings, Kilobytes(1));

    Position tailPosition = { .X = 0, .Y = 0 };
    HashMap* positionsMap = map_NewMap(&arena, 100);
    map_Insert(&arena, positionsMap, PositionToString(&arena, &tailPosition), &tailPosition, sizeof(Position));

    HashMap* positionsMapForNineTail = map_NewMap(&arena, 100);
    map_Insert(&arena, positionsMapForNineTail, PositionToString(&arena, &tailPosition), &tailPosition, sizeof(Position));

    List* tenPositionsList = InitTenPositionsList(&arena);

    List* fileLines = file_read_lines_as_mystrings_max_line_length(&arena, "../../data/09.txt", 10);
    list_Loop(fileLines, MyString, node, line)
    {
        str_TrimEndWhiteSpace(line);
        Motion motion = ParseMotion(&arena, line);
        for (size_t index = 0; index < motion.Count; index++)
        {
            size_t knotIndex = 0;
            bool tailMoved = false;
            Position oldHeadPos = {};

            list_Loop(tenPositionsList, Position, posNode, pos)
            {
                bool currentPositionIsHead = knotIndex++ == 0;
                if (currentPositionIsHead) 
                {
                    oldHeadPos.X = pos->X;
                    oldHeadPos.Y = pos->Y;
                    ApplyMotionOneStep(&motion, pos);
                }
                else 
                {
                    Position oldPos = { .X = pos->X, .Y = pos->Y };
                    Position* head = (Position*)posNode->Prev->Data;
                    tailMoved = ApplyTailFollow(&oldHeadPos, head, pos);
                    oldHeadPos = oldPos;
                }
                
                if (tailMoved)
                {
                    MyString* positionString = PositionToString(&arenaTmpStrings, pos);

                    if (knotIndex == 2) 
                    {
                        map_Insert(&arena, positionsMap, positionString, pos, sizeof(Position));
                    }
                    else if (knotIndex == 10)
                    {
                        map_Insert(&arena, positionsMapForNineTail, positionString, pos, sizeof(Position));
                    }

                    memory_arena_reset(&arenaTmpStrings);
                }
            }
        }
    }

    printf("09a: %lu\n", positionsMap->Count);
    printf("09b: %lu\n", positionsMapForNineTail->Count);
    
    memory_arena_free(&arena);
    memory_arena_free(&arenaTmpStrings);
}

Motion ParseMotion(MemoryArena* arena, MyString* fileLine)
{
    List* splitResult = str_SplitByChar(fileLine, ' ', arena);
    assert(splitResult->Size == 2);

    MyString* direction = list_GetStruct(splitResult, 0, MyString);
    MyString* count = list_GetStruct(splitResult, 1, MyString);

    Motion motion;
    str_ToSizeT(count, &motion.Count);
    motion.Direction = direction->Characters[0];

    return motion;
}

MyString* PositionToString(MemoryArena* arena, Position* position)
{
    MyString* xStr = str_IntToString(arena, position->X);
    MyString* yStr = str_IntToString(arena, position->Y);

    MyString* join = str_InitLimit(arena, ",", 1);
    MyString* result = str_Concat(arena, xStr, join);
    result = str_Concat(arena, result, yStr);

    return result;
}

void ApplyMotionOneStep(Motion* motion, Position* headPosition)
{
    int oldHeadX = headPosition->X;
    int oldHeadY = headPosition->Y;

    if (motion->Direction == 'U')
    {
        headPosition->Y++;
    }
    else if(motion->Direction == 'D')
    {
        headPosition->Y--;
    }
    else if(motion->Direction == 'R')
    {
        headPosition->X++;
    }
    else if(motion->Direction == 'L')
    {
        headPosition->X--;
    }
}


bool ApplyTailFollow(Position* oldHeadPos, Position* currentHeadPos, Position* tailPosition)
{
    int xDistance = abs(currentHeadPos->X - tailPosition->X);
    int yDistance = abs(currentHeadPos->Y - tailPosition->Y);
    bool tailMoved = false;
    int xMove = 0;
    if (currentHeadPos->X > tailPosition->X)
        xMove = 1;
    else if (currentHeadPos->X < tailPosition->X)
        xMove = -1;
    
    int yMove = 0;
    if (currentHeadPos->Y > tailPosition->Y)
        yMove = 1;
    else if (currentHeadPos->Y < tailPosition->Y)
        yMove = -1;

    if (xDistance > 1) 
    {
        tailPosition->X += xMove;
        tailPosition->Y += yMove;
        tailMoved = true;
    }
    else if (yDistance > 1)
    {
        tailPosition->X += xMove;
        tailPosition->Y += yMove;
        tailMoved = true;
    }

    return tailMoved;
}

List* InitTenPositionsList(MemoryArena* arena)
{
    List* positionsList = list_Init(arena, sizeof(Position));

    for (size_t index = 0; index < 10; index++)
    {
        Position p = { .X = 0, .Y = 0 };
        list_Add(arena, positionsList, &p, sizeof(Position));
    }

    return positionsList;
}

void PrintRopeSim(MemoryArena* arena, HashMap* map)
{
    List* keys = map_GetKeys(arena, map);
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;

    list_Loop(keys, MyString, keyNode, keyStr)
    {
        List* split = str_SplitByChar(keyStr, ',', arena);

        MyString* xStr = list_Get(split, 0);
        MyString* yStr = list_Get(split, 1);

        int X, Y;
        str_ToInt(xStr, &X);
        str_ToInt(yStr, &Y);

        if (X > maxX) 
        {
            maxX = X;
        }
        if (X < minX) 
        {
            minX = X;
        }
        if (Y > maxY) 
        {
            maxY = Y;
        }
        if (Y < minY) 
        {
            minY = Y;
        }
    }
    Position p = {};
    int lineIndex = 0;
    for (int y = maxY; y >= minY; y--)
    {
        printf("%02d: ", lineIndex++);
        for (int x = minX; x <= maxX; x++)
        {
            p.X = x;
            p.Y = y;
            MyString* str = PositionToString(arena, &p);
            if (map_Contains(map, str)) 
            {
                printf("#");
            }
            else 
            {
                printf(".");
            }
        }
        printf("\n");
    }
}
