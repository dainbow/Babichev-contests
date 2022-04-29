#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t CalcLogarithm(uint32_t elem) {
    return 31 - __builtin_clz(elem);
}

void FillRMQ(int32_t** sparseTable, uint32_t elemAmount, uint32_t logOfElem) {
    for (uint32_t curTable = 1; curTable < 1 + logOfElem; curTable++) {
        sparseTable[curTable] = calloc(elemAmount, sizeof(int32_t));
        uint32_t shift = 1 << (curTable - 1);

        for (uint32_t curElem = 0; curElem < elemAmount - shift; curElem++) {
            sparseTable[curTable][curElem] = 
                (sparseTable[curTable - 1][curElem] > sparseTable[curTable - 1][curElem + shift]) ?
                 sparseTable[curTable - 1][curElem] : sparseTable[curTable - 1][curElem + shift];
        }

        // for (uint32_t curElem = elemAmount - shift; curElem < elemAmount; curElem++) {
        //     sparseTable[curTable][curElem] = sparseTable[curTable - 1][curElem];
        // }
    }
}

int32_t CalcRMQ(int32_t** sparseTable, uint32_t left, uint32_t right) {
    uint32_t curLog = CalcLogarithm(1 + right - left);

    int32_t leftThing  = sparseTable[curLog][left];
    int32_t rightThing = sparseTable[curLog][(1 + right) - (1 << curLog)];

    return (leftThing > rightThing) ? leftThing : rightThing; 
}

int main() {
    uint32_t elemAmount = 0;
    uint32_t rqstAmount = 0;

    scanf("%u %u", &elemAmount, &rqstAmount);

    uint32_t logOfElem = CalcLogarithm(elemAmount);
    // printf("LOG IS %u\n", logOfElem);

    int32_t** sparseTable = calloc(logOfElem + 1, sizeof(int32_t*));

    sparseTable[0] = calloc(elemAmount, sizeof(int32_t));
    for (uint32_t curElem = 0; curElem < elemAmount; curElem++) {
        scanf("%d", sparseTable[0] + curElem);
    }
    FillRMQ(sparseTable, elemAmount, logOfElem);

    // for (uint32_t curTable = 0; curTable < 1 + logOfElem; curTable++) {
    //     for (uint32_t curElem = 0; curElem < elemAmount; curElem++) {
    //         printf("%d ", sparseTable[curTable][curElem]);
    //     }
    //     printf("\n");
    // }

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        uint32_t left  = 0;
        uint32_t right = 0;
        scanf("%u %u", &left, &right);

        printf("%d\n", CalcRMQ(sparseTable, left, right));
    }

    for (uint32_t curTable = 0; curTable < 1 + logOfElem; curTable++) {
        free(sparseTable[curTable]);
    }

    free(sparseTable);
}
