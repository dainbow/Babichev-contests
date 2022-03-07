#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct intAndIdx {
    int32_t value;
    
    int32_t index;
};

int32_t structCmp(const void* elem1, const void* elem2) {
    return ((struct intAndIdx*)elem1)->value > ((struct intAndIdx*)elem2)->value;
}

int main() {
    int32_t amount = 0;
    scanf("%d", &amount);

    struct intAndIdx* firstArr  = calloc(sizeof(struct intAndIdx), amount);
    struct intAndIdx* secondArr = calloc(sizeof(struct intAndIdx), amount);

    for (uint32_t curElem = 0; curElem < amount; curElem++) {
        scanf("%d", &((firstArr + curElem)->value));
        (firstArr + curElem)->index = curElem;
    }

    for (uint32_t curElem = 0; curElem < amount; curElem++) {
        scanf("%d", &((secondArr + curElem)->value));
        (secondArr + curElem)->index = curElem;
    }

    qsort(firstArr, amount, sizeof(struct intAndIdx), structCmp);
    qsort(secondArr, amount, sizeof(struct intAndIdx), structCmp);

    // for (uint32_t curElem = 0; curElem < amount; curElem++) {
    //     printf("%d ", (firstArr + curElem)->value);
    // }
    // printf("\n");


    // for (uint32_t curElem = 0; curElem < amount; curElem++) {
    //     printf("%d ", (secondArr + curElem)->value);
    // }
    // printf("\n");

    int32_t maxSum = 0;
    int32_t maxI   = 0;
    int32_t maxJ   = 0;

    for (int32_t curElemFirst = amount - 1; curElemFirst >= 0; curElemFirst--) {
        for (int32_t curElemSecond = amount - 1; curElemSecond >= 0; curElemSecond--) {
            if ((firstArr[curElemFirst].value + secondArr[curElemSecond].value) > maxSum) {
                if (firstArr[curElemFirst].index <= secondArr[curElemSecond].index) {
                    maxSum = firstArr[curElemFirst].value + secondArr[curElemSecond].value;

                    maxI = firstArr[curElemFirst].index;
                    maxJ = secondArr[curElemSecond].index;
                }
            }
            else if ((firstArr[curElemFirst].value + secondArr[curElemSecond].value) == maxSum) {
                if (firstArr[curElemFirst].index <= secondArr[curElemSecond].index) {
                    if (firstArr[curElemFirst].index < maxI) {
                        maxI = firstArr[curElemFirst].index;
                        maxJ = secondArr[curElemSecond].index;
                    }
                    else if ((firstArr[curElemFirst].index == maxI) && (secondArr[curElemSecond].index < maxJ)) {
                        maxI = firstArr[curElemFirst].index;
                        maxJ = secondArr[curElemSecond].index;
                    }
                }
            }
            else
                break;
        }
    }

    printf("%d %d", maxI, maxJ);


    free(secondArr);
    free(firstArr);
}

