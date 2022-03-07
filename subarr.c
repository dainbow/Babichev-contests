#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    int64_t arrSize     = 0;
    int64_t exampleSize = 0;

    scanf("%ld %ld", &arrSize, &exampleSize);

    uint64_t* numberArr  = calloc(sizeof(uint64_t), arrSize);

    uint64_t* exampleArr = calloc(sizeof(uint64_t), exampleSize);

    if (arrSize)
        scanf("%lu", numberArr);

    for (uint64_t curNumber = 1; curNumber < arrSize; curNumber++) {
        scanf("%lu", numberArr + curNumber);
    }

    for (uint64_t curExample = 0; curExample < exampleSize; curExample++) {
        scanf("%lu", exampleArr + curExample);
    }

    // for (uint64_t curNumber = 0; curNumber < arrSize; curNumber++) {
    //     printf("%lu ", numberArr[curNumber]);
    // }
    // printf("\n");

    // sum[i, j] = arr[j] - arr[i - 1]

    for (uint64_t curExample = 0; curExample < exampleSize; curExample++) {
        uint64_t curSum = 0;
        int64_t leftBorder = 0;
        int64_t rightBorder = 0;

        uint8_t foundFlag = 0;

        for (; rightBorder < arrSize + 1; rightBorder++) {
            while ((curSum > exampleArr[curExample]) && (leftBorder < rightBorder)) {
                curSum -= numberArr[leftBorder];
                leftBorder++;
            }

            if (curSum == exampleArr[curExample]) {
                foundFlag = 1;

                break;
            }

            curSum += (rightBorder != (arrSize + 1)) * numberArr[rightBorder];
        }

        if (foundFlag)
            printf("%lu %lu\n", leftBorder + 1, rightBorder + 1);
        else
            printf("Not found\n");
    }

    free(numberArr);
    free(exampleArr);
}

