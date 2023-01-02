#ifndef TWELVE_H
#define TWELVE_H

#include "util/list.h"
#include "util/arraylist.h"
#include "util/my_string.h"
#include "util/memory.h"

struct Node
{
    char Character;
    int Height;
    size_t Index;
    ArrayList* AdjacentNodes;
};
typedef struct Node Node;

struct Graph
{
    Node* StartNode;
    Node* DestinationNode;
    ArrayList* Nodes;
    size_t MapWidth;
    size_t MapHeight;
};
typedef struct Graph Graph;

void TwelveSolve(MemoryArena* arena, List* fileLines);
char* ParseHeightMap(MemoryArena* arena, List* fileLines, size_t gridWidth, size_t gridHeight);

size_t ShortestPathFromTo(Node* from, Node* to);
void CalculateDistances(MemoryArena* arena, Node* start, Node* destination, size_t count, int* distances);
void AddAdjacentNodeIfEligible(MemoryArena* arena, ArrayList* nodes, Node* currentNode, int x, int y, size_t mapWidth, size_t mapHeight);
Graph* BuildGraphFromHeightMap(MemoryArena* arena, char* heightMap, size_t mapWidth, size_t mapHeight);

void PrintDistancesFromStart(Graph* graph, int* distances);

#endif