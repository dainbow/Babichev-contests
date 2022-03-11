#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    uint32_t aAmount   = 0;
    uint32_t bAmount   = 0;
    uint32_t arrLength = 0;

    scanf("%u %u %u", &aAmount, &bAmount, &arrLength);

    uint32_t** aArrs = calloc(aAmount, sizeof(uint32_t*));
    uint32_t** bArrs = calloc(bAmount, sizeof(uint32_t*));

    for (uint32_t curA = 0; curA < aAmount; curA++) {
        aArrs[curA] = calloc(arrLength, sizeof(uint32_t));

        for (uint32_t curElem = 0; curElem < arrLength; curElem++) {
            scanf("%u", aArrs[curA] + curElem);
        }
    }

    for (uint32_t curB = 0; curB < bAmount; curB++) {
        bArrs[curB] = calloc(arrLength, sizeof(uint32_t));

        for (uint32_t curElem = 0; curElem < arrLength; curElem++) {
            scanf("%u", bArrs[curB] + curElem);
        }
    }

    uint32_t rAmount = 0;
    scanf("%u", &rAmount);

    uint32_t* answers = calloc(rAmount, sizeof(uint32_t));

    for (uint32_t curReq = 0; curReq < rAmount; curReq++) {
        uint32_t aIdx = 0;
        uint32_t bIdx = 0;

        scanf("%u %u", &aIdx, &bIdx);
        aIdx--;
        bIdx--;

        int32_t left  = 0;
        int32_t right = arrLength - 1;
        int32_t middle = 0;

        uint32_t answer = 0;

        while (left <= right) {
            middle = (left + right) / 2;
            int32_t diff = (int32_t)aArrs[aIdx][middle] - (int32_t)bArrs[bIdx][middle];

            if (diff < 0) {
                left = middle + 1;
            }
            else if (diff > 0) {
                right = middle - 1;
            }
            else {
                break;
            }
        }

        int32_t diff1 = INT32_MAX;
        if (middle > 0) {
            diff1 = (aArrs[aIdx][middle - 1] > bArrs[bIdx][middle - 1]) ? aArrs[aIdx][middle - 1] : bArrs[bIdx][middle - 1];
        }

        int32_t diff2 = (aArrs[aIdx][middle] > bArrs[bIdx][middle]) ? aArrs[aIdx][middle] : bArrs[bIdx][middle];

        int32_t diff3 = INT32_MAX;
        if (middle < arrLength - 1)
            diff3 = (aArrs[aIdx][middle + 1] > bArrs[bIdx][middle + 1]) ? aArrs[aIdx][middle + 1] : bArrs[bIdx][middle + 1];

        answer = (diff1  <= diff2) ? middle - 1  : middle;
        diff1  = (diff1  < diff2) ? diff1       : diff2;

        answer = (diff1 <= diff3) ? answer : middle + 1;
        
        answers[curReq] = answer + 1;
    }

    for (uint32_t curReq = 0; curReq < rAmount; curReq++) {
        printf("%u\n", answers[curReq]);
    }

    for (uint32_t curA = 0; curA < aAmount; curA++) {
        free(aArrs[curA]);
    }

    for (uint32_t curB = 0; curB < bAmount; curB++) {
        free(bArrs[curB]);
    }

    free(aArrs);
    free(bArrs);
    free(answers);
}