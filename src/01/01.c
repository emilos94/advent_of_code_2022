#include <stdio.h>
#include <stdlib.h>
#include "../util/file_util.h"

void read_calorie_counts(size_t* elfIndex, size_t* calorieCounts);
void sort(size_t* calorieCounts, size_t numCounts);

void quickSort(size_t* calorieCounts, size_t low, size_t high);
size_t partition(size_t* calorieCounts, size_t low, size_t high);
void swap(size_t* array, size_t i, size_t j);

#define VERBOSE 0

int main(void) {
    size_t calorieCounts[1000];
    size_t elfIndex = 0;
    read_calorie_counts(&elfIndex, calorieCounts);

    sort(calorieCounts, elfIndex);

    printf("01a: %zu\n", calorieCounts[elfIndex]);
    printf("01b: %zu\n", calorieCounts[elfIndex] + calorieCounts[elfIndex-1] + calorieCounts[elfIndex-2]);

    return 0;
}

void read_calorie_counts(size_t* elfIndex, size_t* calorieCounts) 
{
    FILE *textfile;
    char line[MAX_LINE_LENGTH];
     
    textfile = fopen("../../data/01.txt", "r");
    if(textfile == NULL)
    {
        return;
    }

    size_t currentNumber = 0;
    size_t largestNumber = 0;
    
    while(fgets(line, MAX_LINE_LENGTH, textfile))
    {
        bool IsEmptyLine = line[0] == '\n';
        if(IsEmptyLine) 
        {
            calorieCounts[(*elfIndex)++] = currentNumber;
            currentNumber = 0;
        }
        else 
        {
            int number;
            sscanf(line, "%d", &number);
            currentNumber += number;
        }
    }

    calorieCounts[(*elfIndex)] = currentNumber;
    fclose(textfile);
}


void sort(size_t* array, size_t numCounts)
{
    quickSort(array, 0, numCounts);
}

void quickSort(size_t* array, size_t low, size_t high)
{
    if (low >= high || low < 0)
        return;

    size_t pIndex = partition(array, low, high);

    if (pIndex == 0) return;

    quickSort(array, low, pIndex-1);
    quickSort(array, pIndex+1, high);
}

size_t partition(size_t* array, size_t low, size_t high)
{
    if (VERBOSE) 
    {
        printf("Partitioning (%zu, %zu)\n", low, high);
    }
    size_t pivot = array[high];

    size_t i = low - 1;

    for(size_t j = low; j <= high-1; j++)
    {
        if (array[j] < pivot) 
        {
            i++;
            swap(array, i, j);
        }
    }

    i++;
    swap(array, i, high);
    return i;
}

void swap(size_t* array, size_t i, size_t j) 
{
    if (VERBOSE) 
    {
        printf("Swap (%zu, %zu)\n", i, j);
    }
    size_t temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
