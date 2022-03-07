#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t intCmp(const void* elem1, const void* elem2) {
    return *(int64_t*)elem1 >= *(int64_t*)elem2;
}

int main() {
    uint64_t arrSize        = 0;
    uint64_t examplesAmount = 0;

    scanf("%lu %lu", &arrSize, &examplesAmount);

    int64_t* numberArr  = calloc(sizeof(int64_t), arrSize);
    int64_t* exampleArr = calloc(sizeof(int64_t), examplesAmount);

    for (uint64_t curElem = 0; curElem < arrSize; curElem++) {
        scanf("%ld", numberArr + curElem);
    }

    for (uint64_t curElem = 0; curElem < examplesAmount; curElem++) {
        scanf("%ld", exampleArr + curElem);
    }

    qsort(numberArr, arrSize, sizeof(int64_t), intCmp);

    for (uint64_t curExample = 0; curExample < examplesAmount; curExample++) {
        int64_t firstNum  = INT64_MIN;
        int64_t secondNum = INT64_MIN;

        uint64_t leftPtr  = 0;
        uint64_t rightPtr = arrSize - 1;

        while (leftPtr != rightPtr) {
            int64_t curSum = numberArr[leftPtr] + numberArr[rightPtr];

            if (curSum < exampleArr[curExample]) {
                leftPtr++;
            }
            else if (curSum > exampleArr[curExample]) {
                rightPtr--;
            }
            else {
                firstNum  = numberArr[leftPtr];
                secondNum = numberArr[rightPtr];

                break;
            }
        }

        if ((firstNum == INT64_MIN) && (secondNum == INT64_MIN)) {
            printf("Not found\n");
        }
        else {
            printf("%ld %ld\n", firstNum, secondNum);
        }
    } 

    free(exampleArr);
    free(numberArr);
}
