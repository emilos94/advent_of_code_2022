#include "./my_string.h"

List* str_SplitByChar(MyString* str, char splitChar, MemoryArena* arena)
{
    List* resultList = list_Init(arena, sizeof(MyString));

    size_t lastSplitIndex = 0;

    for (size_t i = 0; i < str->Length; i++)
    {
        if (str->Characters[i] == splitChar) 
        {
            size_t strLength = 0;
            if (lastSplitIndex == 0)
            {
                strLength = i;
            }
            else 
            {
                strLength = i - lastSplitIndex;
            }
            
            MyString* s = memory_AllocateStruct(arena, MyString);
            s->Length = strLength;
            s->Characters = memory_allocate(arena, sizeof(char) * strLength);
            memcpy(s->Characters, str->Characters + lastSplitIndex, strLength);
            list_Add(arena, resultList, s, sizeof(MyString));
            lastSplitIndex = i;
        }
    }

    size_t lengthOfStr = str->Length - lastSplitIndex - 1;
    MyString* s = memory_AllocateStruct(arena, MyString);
    s->Length = lengthOfStr;
    s->Characters = memory_allocate(arena, sizeof(char) * lengthOfStr);
    memcpy(s->Characters, str->Characters + lastSplitIndex + 1, lengthOfStr);
    list_Add(arena, resultList, s, sizeof(MyString));
    
    return resultList;
}


MyString* str_Init(MemoryArena* arena, char* characters)
{
    MyString* str = memory_AllocateStruct(arena, MyString);
    size_t length = 0;
    while ((characters[length++]) != '\0');

    str->Length = length-1;
    str->Characters = memory_AllocateArray(arena, char, length);
    memcpy(str->Characters, characters, str->Length);

    return str;
}


void str_TrimEndWhiteSpace(MyString* str)
{
    for (int i = str->Length-1; i >= 0; i--)
    {
        bool IsWhiteSpace = 
            str->Characters[i] == ' ' ||
            str->Characters[i] == '\n' ||
            str->Characters[i] == '\t';

        if (IsWhiteSpace) 
        {
            str->Characters[i] = '\0';
            str->Length--;
        }
    }
}

bool str_ToInt(MyString* str, int* number)
{
    int result = 0;
    int tensPlace = 1;
    for (int index = str->Length-1; index >= 0; index--)
    {
        char currentChar = str->Characters[index];

        bool isNumber = currentChar >= '0' && currentChar <= '9';
        if (isNumber)
        {
            result += ((int)currentChar - '0') * tensPlace;
        }
        else if (index == 0)
        {
            if (currentChar == '-')
            {
                result *= -1;
                (*number) = result;
                return true;
            }
        }
        else 
        {
            return false;
        }

        tensPlace *= 10;
    }

    (*number) = result;
    return true;
}
