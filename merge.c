#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main() {
    uint64_t arrSize = 0;
    scanf("%lu", &arrSize);

    int64_t* numberArr = calloc(sizeof(int64_t), arrSize);
    uint64_t* oddStates = calloc(sizeof(uint64_t), ((arrSize + 1) >> 6) + (((arrSize + 1) & 63) != 0));

    for (uint64_t curElem = 0; curElem < arrSize; curElem++) {
        scanf("%ld", numberArr + curElem);
        oddStates[curElem >> 6] |= ((curElem + 1) & 1) << (curElem & 63);
    }
    oddStates[arrSize >> 6] |= ((arrSize + (uint64_t)1) & (uint64_t)1) << (arrSize & (uint64_t)63);

    int64_t tempValue    = 0;
    int64_t addSum       = 0;
    
    for (uint32_t curElem = 0; curElem < arrSize; curElem++) {
        scanf("%ld", &tempValue);

        int64_t left   = 0;
        int64_t right  = arrSize - 1;
        int64_t middle = 0;

        while (left <= right) {
            middle = (left + right) / 2;

            if (numberArr[middle] > tempValue) {
                right = middle - 1;
            }
            else if (numberArr[middle] < tempValue) {
                left = middle + 1;
            }
            else {
                break;
            }
        }
        // if (curElem == arrSize - 1) {
        //     printf("DEBUG\n");
        // }

        while ((numberArr[middle] <= tempValue) && (middle < arrSize)) {
            middle++;
        }
        addSum += ((oddStates[middle >> 6] & ((uint64_t)1 << (middle & 63))) >> (middle & 63)) * tempValue;


        oddStates[middle >> 6] ^= UINT64_MAX - (((uint64_t)1 << (middle & 63)) - 1);

        for (uint32_t curLong = (middle >> 6) + 1; curLong < ((arrSize + 1) >> 6) + (((arrSize + 1) & 63) != 0); curLong++) {
            oddStates[curLong] ^= UINT64_MAX;
        }
    }

    for (uint64_t curElem = 0; curElem < arrSize; curElem++) {
        assert((((oddStates[curElem >> 6] & ((uint64_t)1 << (curElem & (uint64_t)63))) >> (curElem & (uint64_t)63))) <= 1);
        addSum += (((oddStates[curElem >> 6] & ((uint64_t)1 << (curElem & (uint64_t)63))) >> (curElem & (uint64_t)63))) * numberArr[curElem];
    }

    printf("%ld\n", addSum % 1000000000);


    free(oddStates);
    free(numberArr);
}