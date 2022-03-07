#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t mergeSort(int32_t* array, uint64_t left, uint64_t right) {
    if (left != right) {
        uint64_t invCount = 0;
        uint64_t middle = (left + right) / 2;

        invCount += mergeSort(array, left, middle);
        invCount += mergeSort(array, middle + 1, right);

        int32_t* arrayLeft  = calloc(middle + 1 - left, sizeof(int32_t));
        int32_t* arrayRight = calloc(right - middle, sizeof(int32_t));

        uint64_t leftIdx  = 0;
        uint64_t rightIdx = 0;

        for (uint64_t curElem = left; curElem < middle + 1; curElem++) {
            arrayLeft[leftIdx++] = array[curElem];
        } 

        for (uint64_t curElem = middle + 1; curElem < right + 1; curElem++) {
            arrayRight[rightIdx++] = array[curElem];
        } 
        
        uint64_t bigIdx   = left;
        leftIdx           = 0;
        rightIdx          = 0;

        while ((leftIdx < middle + 1 - left) && (rightIdx < right - middle)) {
            if (arrayLeft[leftIdx] <= arrayRight[rightIdx]) {
                array[bigIdx++] = arrayLeft[leftIdx++];
            }
            else {
                array[bigIdx++] = arrayRight[rightIdx++];
                invCount += middle + 1 - left - leftIdx;
            }
        }
// -43 - 19 0 -20
        while (leftIdx < middle + 1 - left) {
            array[bigIdx++] = arrayLeft[leftIdx++];
        }

        while (rightIdx < right - middle) {
            array[bigIdx++] = arrayRight[rightIdx++];
        }

        free(arrayRight);
        free(arrayLeft);

        return invCount;
    }

    return 0;
}

int main() {
    uint64_t amount = 0;
    scanf("%lu", &amount);

    int32_t* numArr  = calloc(amount, sizeof(int32_t));

    for (uint64_t curElem = 0; curElem < amount; curElem++) {
        scanf("%d", numArr + curElem);
    }

    printf("%lu", mergeSort(numArr, 0, amount - 1));

    free(numArr);
    
}