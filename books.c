#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define AMOUNT 255

int intCmp(const void* elem1, const void* elem2) {
    return *(uint32_t*)elem1 > *(uint32_t*)elem2;
} 

int main() {
    uint64_t tomsAmount = 0;
    uint64_t listAmount = 0;

    scanf("%lu %lu", &tomsAmount, &listAmount);

    uint32_t* lists       = calloc(listAmount + 1, sizeof(uint32_t));
    lists++;

    uint64_t maxList = 0;

    uint32_t tempValue = 0;



    for (int64_t curList = 0; curList < listAmount; curList++) {
        scanf("%u", &tempValue);

        maxList = (tempValue > maxList) ? tempValue : maxList;
        lists[curList] = lists[curList - 1] + tempValue;
    }

    int64_t left  = maxList;
    int64_t right = lists[listAmount - 1];

    int64_t middle = 0;
    int64_t answer = 0;

    while (left <= right) {
        middle = (left + right) / 2;
        int64_t lastTomIdx   = 0;
        int64_t curTomAmount = 0;

        for (uint32_t curList = 0; curList < listAmount; curList++) {
            if ((lists[curList] - lists[lastTomIdx - 1]) > middle) {
                lastTomIdx = curList;
                curTomAmount++;
            }
        }
        curTomAmount++;

        if (curTomAmount > tomsAmount)
            left = middle + 1;
        else {
            answer = middle;
            right = middle - 1;
        }
    }

    printf("%ld\n", answer); 

    free(lists - 1);
}   

