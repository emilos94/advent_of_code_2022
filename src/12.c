#include "12.h"

void TwelveSolve(MemoryArena* arena, List* fileLines)
{
    size_t gridWidth = (list_PeekFrontValue(fileLines, MyString))->Length;
    size_t gridHeight = fileLines->Size;

    char* heightMap = ParseHeightMap(arena, fileLines, gridWidth, gridHeight);

    Graph* graph = BuildGraphFromHeightMap(arena, heightMap, gridWidth, gridHeight);
    int distances[graph->Nodes->Count];
    CalculateDistances(arena, graph->StartNode, graph->DestinationNode, graph->Nodes->Count, &distances);

    printf("12a: %d\n", distances[graph->DestinationNode->Index]);

    memory_ArenaBlock(tmpArena, Megabytes(1))
    {
        size_t minDistanceFromA = -1;
        arrlist_Foreach(graph->Nodes, Node, node)
        {
            if (node->Character == 'a')
            {
                CalculateDistances(&tmpArena, node, graph->DestinationNode, graph->Nodes->Count, &distances);
                int nodeDistance = distances[graph->DestinationNode->Index];
                if (nodeDistance < minDistanceFromA)
                {
                    minDistanceFromA = nodeDistance;
                }
            }
            memory_arena_reset(&tmpArena);
        }
        printf("12b: %lu\n", minDistanceFromA);
    }
}

char* ParseHeightMap(MemoryArena* arena, List* fileLines, size_t gridWidth, size_t gridHeight)
{
    char* heightMap = memory_AllocateArray(arena, char, gridWidth * gridHeight);

    size_t currentHeight = 0;
    list_Loop(fileLines, MyString, lineNode, str)
    {
        for (size_t charIndex = 0; charIndex < gridWidth; charIndex++)
        {
            char elevation = str->Characters[charIndex];
            size_t mapIndex = currentHeight * gridWidth + charIndex;
            heightMap[mapIndex] = elevation;   
        }
        currentHeight++;
    }

    return heightMap;
}

void CalculateDistances(MemoryArena* arena, Node* start, Node* destination, size_t count, int* distances)
{      
    for (size_t distIndex = 0; distIndex < count; distIndex++)
    {
        distances[distIndex] = -1;
    }

    List* queue = list_Init(arena, sizeof(Node));

    list_Add(arena, queue, start, sizeof(Node));

    distances[start->Index] = 0;

    while (queue->Size > 0)
    {
        Node* current = list_PopFrontValue(queue, Node);

        arrlist_Foreach(current->AdjacentNodes, Node, adjacentNode)
        {
            bool hasNotBeenVisisted = distances[adjacentNode->Index] == -1;
            if (hasNotBeenVisisted)
            {
                distances[adjacentNode->Index] = distances[current->Index] + 1;
                list_Add(arena, queue, adjacentNode, sizeof(Node));
            }
            else 
            {
                int currentDistance = distances[adjacentNode->Index];

                if (currentDistance > distances[current->Index] + 1)
                {
                    distances[adjacentNode->Index] = distances[current->Index] + 1;
                }
            }
        }
    }
    
}

Graph* BuildGraphFromHeightMap(MemoryArena* arena, char* heightMap, size_t mapWidth, size_t mapHeight)
{   
    Graph* graph = memory_AllocateStruct(arena, Graph);
    graph->Nodes = arrlist_New(arena, mapWidth * mapHeight, sizeof(Node));
    graph->MapHeight = mapHeight;
    graph->MapWidth = mapWidth;

    // initialise nodes
    for (size_t y = 0; y < mapHeight; y++)
    {    
        for (size_t x = 0; x < mapWidth; x++)
        {
            size_t mapIndex = y * mapWidth + x;
            char currentElevation = heightMap[mapIndex];

            Node* node = memory_AllocateStruct(arena, Node);
            node->AdjacentNodes = arrlist_New(arena, 4, sizeof(Node));
            node->Index = mapIndex;
            node->Character = currentElevation;

            if (currentElevation == 'S')
            {
                node->Height = 0;
                graph->StartNode = node;
            }
            else if (currentElevation == 'E')
            {
                node->Height = (int)'z' - (int)'a';
                graph->DestinationNode = node;
            }
            else 
            {
                node->Height = (int)currentElevation - (int)'a';
            }

            arrlist_Add(arena, graph->Nodes, node);
        }
    }

    // set adjacent nodes
    for (int y = 0; y < mapHeight; y++)
    {    
        for (int x = 0; x < mapWidth; x++)
        {
            size_t mapIndex = y * mapWidth + x;
            Node* node = arrlist_GetType(graph->Nodes, Node, mapIndex);

            AddAdjacentNodeIfEligible(arena, graph->Nodes, node, x - 1, y, mapWidth, mapHeight);
            AddAdjacentNodeIfEligible(arena, graph->Nodes, node, x + 1, y, mapWidth, mapHeight);
            AddAdjacentNodeIfEligible(arena, graph->Nodes, node, x, y - 1, mapWidth, mapHeight);
            AddAdjacentNodeIfEligible(arena, graph->Nodes, node, x, y + 1, mapWidth, mapHeight);
        }
    }

    return graph;
}


void AddAdjacentNodeIfEligible(MemoryArena* arena, ArrayList* nodes, Node* currentNode, int x, int y, size_t mapWidth, size_t mapHeight)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return;
    
    size_t index = y * mapWidth + x;
    Node* node = arrlist_GetType(nodes, Node, index);

    int heightDifference = abs(currentNode->Height - node->Height);
    if (currentNode->Height > node->Height || heightDifference <= 1)
    {
        if (!arrlist_Contains(currentNode->AdjacentNodes, node))
        {
            arrlist_Add(arena, currentNode->AdjacentNodes, node);
        }
    }
}

void PrintDistancesFromStart(Graph* graph, int* distances)
{
    memory_ArenaBlock(arena, Megabytes(1))
    {
        char map[graph->Nodes->Count];
        for (size_t i = 0; i < graph->Nodes->Count; i++)
        {
            map[i] = ' ';
        }

        List* queue = list_Init(&arena, sizeof(Node));
        list_Add(&arena, queue, graph->StartNode, sizeof(Node));

        while(queue->Size > 0)
        {
            Node* node = list_PopFrontValue(queue, Node);

            arrlist_Foreach(node->AdjacentNodes, Node, adj)
            {
                char mapChar = map[adj->Index];
                bool visited = mapChar != ' ';

                if (!visited)
                {
                    map[adj->Index] = adj->Character == 'E' || adj->Character == 'S' ? adj->Character : (char)distances[adj->Index];
                    
                    list_Add(&arena, queue, adj, sizeof(Node));
                }
            }
        }

        for (size_t y = 0; y < graph->MapHeight; y++)
        {
            for (size_t x = 0; x < graph->MapWidth; x++)
            {
                size_t index = y * graph->MapWidth + x;
                printf("%02d ", distances[index]);
            }
            printf("\n");
        }
    }
}
