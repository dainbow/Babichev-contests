#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AMOUNT 255

void fast_sort(unsigned *begin, unsigned *end) {
    uint32_t countArr[4][AMOUNT + 2];
    memset(countArr[0], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[1], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[2], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[3], 0, sizeof(uint32_t) * (AMOUNT + 2));

    uint8_t   oddFlag         = 0;
    uint32_t* copyArr         = calloc(sizeof(uint32_t), end - begin - 1);

    for (uint32_t* curPtr = begin; curPtr < end; curPtr++) {
        countArr[0][((*curPtr) & AMOUNT)       + 1]++;
        countArr[1][(((*curPtr) >> 8) & AMOUNT)  + 1]++;
        countArr[2][(((*curPtr) >> 16) & AMOUNT) + 1]++;
        countArr[3][(((*curPtr) >> 24) & AMOUNT) + 1]++;
    }

    for (uint32_t curCount = 2; curCount < AMOUNT + 2; curCount++) {
        countArr[0][curCount] += countArr[0][curCount - 1];
        countArr[1][curCount] += countArr[1][curCount - 1];
        countArr[2][curCount] += countArr[2][curCount - 1];
        countArr[3][curCount] += countArr[3][curCount - 1];
    }

    if (countArr[0][1] == (end - begin)) {
        if (oddFlag) {
            memmove(begin, copyArr, sizeof(uint32_t) * (end - begin));
        }
    
        free(copyArr);

        return;
    }

    if (!oddFlag) {
        for (uint32_t* curPtr = begin; curPtr < end; curPtr++) {
            copyArr[countArr[0][(*curPtr) & AMOUNT]++] = *curPtr;
        }
    }
    else {
        for (uint32_t* curPtr = copyArr; curPtr < copyArr + (end - begin); curPtr++) {
            begin[countArr[0][(*curPtr) & AMOUNT]++] = *curPtr;
        }
    }

    oddFlag ^= 1;

    if (countArr[1][1] == (end - begin)) {
        if (oddFlag) {
            memmove(begin, copyArr, sizeof(uint32_t) * (end - begin));
        }
    
        free(copyArr);

        return;
    }

    if (!oddFlag) {
        for (uint32_t* curPtr = begin; curPtr < end; curPtr++) {
            copyArr[countArr[1][((*curPtr) >> 8) & AMOUNT]++] = *curPtr;
        }
    }
    else {
        for (uint32_t* curPtr = copyArr; curPtr < copyArr + (end - begin); curPtr++) {
            begin[countArr[1][((*curPtr) >> 8) & AMOUNT]++] = *curPtr;
        }
    }

    oddFlag ^= 1;

    if (countArr[2][1] == (end - begin)) {
        if (oddFlag) {
            memmove(begin, copyArr, sizeof(uint32_t) * (end - begin));
        }
    
        free(copyArr);

        return;
    }

    if (!oddFlag) {
        for (uint32_t* curPtr = begin; curPtr < end; curPtr++) {
            copyArr[countArr[2][((*curPtr) >> 16) & AMOUNT]++] = *curPtr;
        }
    }
    else {
        for (uint32_t* curPtr = copyArr; curPtr < copyArr + (end - begin); curPtr++) {
            begin[countArr[2][((*curPtr) >> 16) & AMOUNT]++] = *curPtr;
        }
    }

    oddFlag ^= 1;

    if (countArr[3][1] == (end - begin)) {
        if (oddFlag) {
            memmove(begin, copyArr, sizeof(uint32_t) * (end - begin));
        }
    
        free(copyArr);

        return;
    }
       

    if (!oddFlag) {
        for (uint32_t* curPtr = begin; curPtr < end; curPtr++) {
            copyArr[countArr[3][((*curPtr) >> 24) & AMOUNT]++] = *curPtr;
        }
    }
    else {
        for (uint32_t* curPtr = copyArr; curPtr < copyArr + (end - begin); curPtr++) {
            begin[countArr[3][((*curPtr) >> 24) & AMOUNT]++] = *curPtr;
        }
    }

    oddFlag ^= 1;
    

    if (oddFlag) {
        memmove(begin, copyArr, sizeof(uint32_t) * (end - begin));
    }
    
    free(copyArr);
}

// int main() {

//     fast_sort(array, array+250);
//     // Now array = {1,1,2,3,4,5,6,9}
//     for (uint32_t curElem = 0; curElem < 250; curElem++) {
//         printf("%u ", array[curElem]);
//     }
//     printf("\n");
// }
