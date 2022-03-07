#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    uint32_t amounts1 = 0;
    uint32_t amounts2 = 0;

    scanf("%u %u", &amounts1, &amounts2);

    int32_t* intArr1 = calloc(sizeof(int32_t), amounts1);
    int32_t* intArr2 = calloc(sizeof(int32_t), amounts2);

    int32_t* answArr    = calloc(sizeof(int32_t), (amounts1 > amounts2) ? amounts1 : amounts2);
    uint32_t answLength = 0;

    for (uint32_t curElem = 0; curElem < amounts1; curElem++) {
        scanf("%d", intArr1 + curElem);
    }

    for (uint32_t curElem = 0; curElem < amounts2; curElem++) {
        scanf("%d", intArr2 + curElem);
    }

    for (uint32_t curElem = 0; curElem < amounts2; curElem++) {
        uint32_t left  = 0;
        uint32_t right = amounts1 - 1;

        while (left <= right) {
            if (intArr1[(left + right) / 2] == intArr2[curElem]) {
                answArr[answLength++] = intArr2[curElem]; 

                break;
            }
            else if (intArr1[(left + right) / 2] > intArr2[curElem]) {
                right = (((left + right) / 2) != 0) ? (((left + right) / 2) - 1) : 0;
            }
            else {
                left = ((left + right) / 2) + 1;
            }
        }
    }

    for (uint32_t curElem = 0; curElem < answLength; curElem++) {
        printf("%d ", answArr[curElem]);
    }

    free(intArr1);
    free(intArr2);
    free(answArr);
}
