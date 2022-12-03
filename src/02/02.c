#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/memory.h"
#include "../util/list.h"
#include "../util/file_util.h"

size_t score_for_line(char myChoice, char opponentChoice);
size_t score_for_choice(char choice);
char choice_map_to_abc(char choice);
bool is_win(char myChoice, char opponentChoice);

char get_needed_choice(char opponentChoice, char wantedOutcome);
char map_to_loss(char opponentChoice);
char map_to_win(char opponentChoice);
bool should_win(char wantedOutcome);
bool should_draw(char wantedOutcome);

#define ROCK 'A'
#define PAPER 'B'
#define SCISSOR 'C'

#define SCORE_DRAW 3
#define SCORE_WIN 6

int main(void) 
{
    MemoryArena arena;
    memory_arena_initialize(&arena, Megabytes(10));

    List* fileLines = list_Init(&arena, sizeof(char) * 1000);
    file_read_lines(&arena, fileLines, "../../data/02.txt");

    ListNode* head = fileLines->Head;
    size_t totalScoreA = 0;
    size_t totalScoreB = 0;
    while(head != NULL) 
    {
        char* line = head->Data;
        totalScoreA += score_for_line(line[2], line[0]);

        char neededChoice = get_needed_choice(line[0], line[2]);
        totalScoreB += score_for_line(neededChoice, line[0]);

        head = head->Next;
    }

    printf("02a: %zu\n", totalScoreA);
    printf("02b: %zu\n", totalScoreB);
}

size_t score_for_line(char myChoice, char opponentChoice)
{   
    char _myChoice = choice_map_to_abc(myChoice);
    size_t score = score_for_choice(_myChoice);

    if (opponentChoice == _myChoice)
    {
        score += SCORE_DRAW;
    }
    else if (is_win(_myChoice, opponentChoice))
    {
        score += SCORE_WIN;
    }
    return score;
}

size_t score_for_choice(char choice)
{
    if (choice == ROCK) return 1;
    else if (choice == PAPER) return 2;
    else if (choice == SCISSOR) return 3;

    assert(false);
}

char choice_map_to_abc(char choice)
{
    if (choice == 'X' || choice == 'A') return ROCK;
    else if (choice == 'Y' || choice == 'B') return PAPER;
    else if (choice == 'Z' || choice == 'C') return SCISSOR;

    assert(false);
}


bool is_win(char myChoice, char opponentChoice)
{
    bool IsWin = (myChoice == ROCK && opponentChoice == SCISSOR) ||
                (myChoice == PAPER && opponentChoice == ROCK) ||
                (myChoice == SCISSOR && opponentChoice == PAPER);
    return IsWin;
}

bool should_win(char wantedOutcome)
{
    return wantedOutcome == 'Z';
}

bool should_draw(char wantedOutcome)
{
    return wantedOutcome == 'Y';
}

char get_needed_choice(char opponentChoice, char wantedOutcome)
{
    if (should_win(wantedOutcome)) return map_to_win(opponentChoice);
    if (should_draw(wantedOutcome)) return opponentChoice;
    else return map_to_loss(opponentChoice);
}

char map_to_loss(char opponentChoice)
{
    if (opponentChoice == ROCK) return SCISSOR;
    if (opponentChoice == PAPER) return ROCK;
    if (opponentChoice == SCISSOR) return PAPER;

    assert(false);
}

char map_to_win(char opponentChoice)
{
    if (opponentChoice == ROCK) return PAPER;
    if (opponentChoice == PAPER) return SCISSOR;
    if (opponentChoice == SCISSOR) return ROCK;

    assert(false);
}
