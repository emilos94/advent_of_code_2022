#include "11.h"

int eleven_solve(MemoryArena* arena, List* fileLines)
{
    ArrayList* monkeyList = ParseMonkeys(arena, fileLines);
    size_t monkeyBusinessA = FindMonkeyBusinessValue(arena, DeepCopyMonkeyList(arena, monkeyList), 20, true);
    size_t monkeyBusinessB = FindMonkeyBusinessValue(arena, DeepCopyMonkeyList(arena, monkeyList), 10000, false);

    printf("11a: %lu\n", monkeyBusinessA);
    printf("11b: %lu\n", monkeyBusinessB);

    return 1;
}

size_t FindMonkeyBusinessValue(MemoryArena* arena, ArrayList* monkeyList, size_t numRounds, bool shouldDivideByThree)
{
    for(size_t round = 0; round < numRounds; round++) 
    {   
        arrlist_Foreach(monkeyList, Monkey, monkey) 
        {
            while (monkey->Items->Size > 0) 
            {
                monkey->InspectCount++;

                ULLONG* item = list_PopFrontValue(monkey->Items, ULLONG);

                ULLONG left = GetMonkeyTestValue(item, monkey->Left);
                ULLONG right = GetMonkeyTestValue(item, monkey->Right);

                ULLONG new = 0;
                if (monkey->Operator == '*')
                {
                    new = left * right;
                }
                else if(monkey->Operator == '+')
                {
                    new = left + right;
                }

                if (shouldDivideByThree)
                {
                    new /= 3;
                }

                size_t monkeyThrowIndex = new % monkey->DivisibleBy == 0 && new > 0 ? monkey->MonkeyIndexTrue : monkey->MonkeyIndexFalse;

                Monkey* throwTo = arrlist_GetType(monkeyList, Monkey, monkeyThrowIndex);
                list_Add(arena, throwTo->Items, &new, sizeof(ULLONG));
            }
        }

        if ((round + 1) % 1000 == 0 || round < 20)
        {
            printf("\n====== ROUND %lu ======\n", round + 1);
            arrlist_Foreach(monkeyList, Monkey, monkey)
            {
                PrettyPrintMonkey(monkey);
            }
        }
    }

    arrlist_Sort(monkeyList, &CompareMonkiesByInspectCount);
    Monkey* first = arrlist_GetType(monkeyList, Monkey, 0);
    Monkey* second = arrlist_GetType(monkeyList, Monkey, 1);
    size_t monkeyBusiness = first->InspectCount * second->InspectCount;

    return monkeyBusiness;
}


ULLONG GetMonkeyTestValue(ULLONG* item, MyString* valueS)
{
    if (str_EqualsChars(valueS, "old"))
    {
        return (*item);
    }
    else 
    {
        size_t value;
        str_ToSizeT(valueS, &value);
        return (ULLONG)value;
    }
}

void PrettyPrintMonkey(Monkey* monkey)
{
    printf("Monkey id: %lu, inspectCount: %lu\n", monkey->Index, monkey->InspectCount);
    
    /*printf("  Items: ");
    list_Loop(monkey->Items, size_t, itemNode, item)
    {
        printf("%lu ", (*item));
    }
    printf("\n"); */
}

bool CompareMonkiesByInspectCount(void* either, void* other)
{
    Monkey* eitherM = (Monkey*)either;
    Monkey* otherM = (Monkey*)other;
    bool result = eitherM->InspectCount > otherM->InspectCount;
    return result;
}

ArrayList* ParseMonkeys(MemoryArena* arena, List* fileLines)
{
    ArrayList* monkyList = arrlist_New(arena, 10, sizeof(Monkey));
    Monkey* currentMonkey = memory_AllocateStruct(arena, Monkey);
    currentMonkey->Items = list_Init(arena, sizeof(ULLONG));
    currentMonkey->InspectCount = 0;

    MemoryArena parsingArena;
    memory_arena_initialize(&parsingArena, Kilobytes(10));

    list_Loop(fileLines, MyString, lineNode, str)
    {
        str_TrimStartAndEndWhiteSpace(str);

        if (str_StartsWith(str, "Monkey ", 7)) 
        {
            List* splitResult = str_SplitByChar(str, ' ', &parsingArena);
            MyString* indexStr = str_Substring(&parsingArena, list_GetStruct(splitResult, 1, MyString), 0, 0);
            size_t index;
            str_ToSizeT(indexStr, &index);
            currentMonkey->Index = index;
        }
        else if (str_StartsWith(str, "Starting items:", 15))
        {
            List* splitResult = str_SplitByChar(str, ':', &parsingArena);
            List* valuesAsStrings = str_SplitByChar(list_GetStruct(splitResult, 1, MyString), ',', &parsingArena);

            list_Loop(valuesAsStrings, MyString, valueNode, valueStr)
            {
                str_TrimStartAndEndWhiteSpace(valueStr);
                size_t value;
                str_ToSizeT(valueStr, &value);
                list_Add(arena, currentMonkey->Items, &value, sizeof(ULLONG));
            }
        }
        else if (str_StartsWith(str, "Operation:", 10)) 
        {
            List* splitResult = str_SplitByChar(str, ' ', arena);
            MyString* left = list_GetStruct(splitResult, 3, MyString);
            MyString* operator = list_GetStruct(splitResult, 4, MyString);
            MyString* right = list_GetStruct(splitResult, 5, MyString);

            currentMonkey->Left = left;
            currentMonkey->Operator = operator->Characters[0];
            currentMonkey->Right = right;
        }
        else if (str_StartsWith(str, "Test:", 5)) 
        {
            List* splitResult = str_SplitByChar(str, ' ', &parsingArena);
            MyString* divisibleByStr = list_GetStruct(splitResult, 3, MyString);
            size_t divisibleBy;
            str_ToSizeT(divisibleByStr, &divisibleBy);
            currentMonkey->DivisibleBy = divisibleBy;
        }
        else if (str_StartsWith(str, "If true", 7)) 
        {
            List* splitResult = str_SplitByChar(str, ' ', &parsingArena);
            MyString* monkeyIndexTrueStr = list_GetStruct(splitResult, 5, MyString);
            size_t monkeyIndexTrue;
            str_ToSizeT(monkeyIndexTrueStr, &monkeyIndexTrue);
            currentMonkey->MonkeyIndexTrue = monkeyIndexTrue;
        }
        else if (str_StartsWith(str, "If false", 8)) 
        {
            List* splitResult = str_SplitByChar(str, ' ', &parsingArena);
            MyString* monkeyIndexFalseStr = list_GetStruct(splitResult, 5, MyString);
            size_t monkeyIndexFalse;
            str_ToSizeT(monkeyIndexFalseStr, &monkeyIndexFalse);
            currentMonkey->MonkeyIndexFalse = monkeyIndexFalse;

            arrlist_Add(arena, monkyList, currentMonkey);

            currentMonkey = memory_AllocateStruct(arena, Monkey);
            currentMonkey->Items = list_Init(arena, sizeof(ULLONG));
        }

        memory_arena_reset(&parsingArena);
    }

    memory_arena_free(&parsingArena);

    return monkyList;
}

ArrayList* DeepCopyMonkeyList(MemoryArena* arena, ArrayList* monkeyList)
{
    ArrayList* listCopy = arrlist_Copy(arena, monkeyList);

    arrlist_Foreach(listCopy, Monkey, monkey)
    {
        monkey->Items = list_Copy(arena, monkey->Items);
    }

    return listCopy;
}
