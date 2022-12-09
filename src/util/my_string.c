#include "./my_string.h"

List* str_SplitByChar(MyString* str, char splitChar, MemoryArena* arena)
{
    List* resultList = list_Init(arena, sizeof(MyString));

    size_t lastStartIndex = 0;

    for (size_t i = 0; i < str->Length; i++)
    {
        if (str->Characters[i] == splitChar) 
        {
            size_t strLength = i - lastStartIndex;
            
            MyString* s = str_InitLimit(arena, str->Characters + lastStartIndex, strLength);
            list_Add(arena, resultList, s, sizeof(MyString));
            lastStartIndex = i + 1;
        }
    }

    bool isLastCharacterSplit = str->Characters[str->Length-1] == splitChar;
    if (!isLastCharacterSplit) 
    {
        size_t strLength = str->Length - lastStartIndex;
        MyString* s = str_InitLimit(arena, str->Characters + lastStartIndex, strLength);
        list_Add(arena, resultList, s, sizeof(MyString));
    }
    
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

MyString* str_InitLimit(MemoryArena* arena, char* characters, size_t length)
{
    MyString* str = memory_AllocateStruct(arena, MyString);

    str->Length = length;
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
        else 
        {
            return;
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


bool str_ToSizeT(MyString* str, size_t* number)
{
    size_t result = 0;
    int tensPlace = 1;
    for (int index = str->Length-1; index >= 0; index--)
    {
        char currentChar = str->Characters[index];

        bool isNumber = currentChar >= '0' && currentChar <= '9';
        if (isNumber)
        {
            result += (size_t)((int)currentChar - '0') * tensPlace;
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


bool str_ContainsChar(MyString* str, char c)
{
    for (size_t index = 0; index < str->Length; index++) 
    {
        if (str->Characters[index] == c) return true;
    }

    return false;
}


bool str_StartsWith(MyString* str, char* characters, size_t length)
{
    if(length > str->Length)
    {
        return false;
    }

    for (size_t index = 0; index < length; index++)
    {
        char currentChar = str->Characters[index];
        char currentTestChar = characters[index];
        if (currentChar != currentTestChar)
        {
            return false;
        }
    }

    return true;
}

void str_Print(MyString* str)
{   
    printf("%.*s", (int)str->Length, str->Characters);
}

void str_PrintLine(MyString* str)
{   
    printf("%.*s\n", (int)str->Length, str->Characters);
}


void str_PrintSubString(MyString* str, size_t start, size_t end)
{
    assert(start < str->Length && end < str->Length);

    printf("%.*s", (int)end - start + 1, str->Characters + start);
}


bool str_Equals(MyString* either, MyString* other)
{
    if (either->Length != other->Length) return false;

    for(size_t charIndex = 0; charIndex < either->Length; charIndex++)
    {
        if (either->Characters[charIndex] != other->Characters[charIndex])
        {
            return false;
        }
    }

    return true;
}
