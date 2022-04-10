#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const int32_t CAPACITY = 50000;

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct pair {
    uint32_t* key;
};

struct hashT {
    struct pair* data;
    char*   state;

    uint32_t capacity;
};

#define AMOUNT 255

void fast_sort(unsigned *begin, unsigned *end) {
    uint32_t countArr[4][AMOUNT + 2];
    memset(countArr[0], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[1], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[2], 0, sizeof(uint32_t) * (AMOUNT + 2));
    memset(countArr[3], 0, sizeof(uint32_t) * (AMOUNT + 2));

    uint8_t   oddFlag         = 0;
    uint32_t* copyArr         = calloc(sizeof(uint32_t), end - begin);

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
}; 

int32_t arrCmp(uint32_t* elem1, uint32_t* elem2, const uint32_t arrSize) {
    for (uint32_t curElem = 0; curElem < arrSize; curElem++) {
        if (elem1[curElem] != elem2[curElem])
            return 1;
    }
    return 0;
}

uint32_t H1(uint32_t* s, const uint32_t hashsize, const uint32_t arrSize) {
    uint32_t sum = 0;
    const uint32_t FACTOR = 5;

    for (uint32_t curByte = 0; curByte < arrSize; curByte++) {
        sum <<= FACTOR;
        sum += *s++;
    }

    return sum & hashsize;
}

uint32_t H2(uint32_t* s, const uint32_t hashsize, const uint32_t arrSize) {
    uint32_t h = 0, a = 31415, b = 27183;

    for (uint32_t curByte = 0; curByte < arrSize; curByte++) {
        h = (a * h + *s++) % hashsize;
        a = a * b % (hashsize - 1);
    }

    return h;
}

struct hashT* newHT(uint64_t capacity) {
    struct hashT* h = calloc(1, sizeof(struct hashT));
    if (h == NULL)
        return h;

    h->data  = calloc(h->capacity = capacity, sizeof(h->data[0]));
    h->state = calloc(capacity, sizeof(h->state[0]));
    
    if ((h->data == NULL) || (h->state == NULL)) {
        free(h->state);
        free(h->data);
        free(h);

        return NULL;
    }
    
    return h;
}

int32_t findHT(struct hashT* hT, uint32_t* key, const uint32_t arrSize) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity, arrSize), increment = H2(key, hT->capacity, arrSize), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (arrCmp(hT->data[curPlace].key, key, arrSize) == 0) {
                    return curPlace;
                }
                break;
            }
            case EMPTY: {
                return -1;
            }

            case DELETED: 
                break;
            default:
                assert(0);
        }
    }

    return 0;
}

void insertHT(struct hashT* hT, uint32_t* key, const uint32_t arrSize) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity, arrSize), increment = H2(key, hT->capacity, arrSize), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
          if (hT->state[curPlace] != BUSY) {
              hT->data[curPlace].key   = key;
              hT->state[curPlace]      = BUSY;
              
              return;
          }
    }
}

void eraseHT(struct hashT* hT, uint32_t* key, const uint32_t arrSize) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity, arrSize), increment = H2(key, hT->capacity, arrSize), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (arrCmp(hT->data[curPlace].key, key, arrSize) == 0) {
                    hT->state[curPlace] = DELETED;

                    free(hT->data[curPlace].key);
                    return;
                }
                break;
            }
            case EMPTY: {
                return;
            }

            case DELETED: 
                break;
            default:
                assert(0);
        }
    }
}

struct hashT* deleteHT(struct hashT* hT) {
    if (hT == NULL)
        return NULL;

    for (uint32_t curElem = 0; curElem < hT->capacity; curElem++) {
        if (hT->state[curElem] == BUSY) {
            free(hT->data[curElem].key);
        }
    }

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;    
}

int main() {
    uint32_t etCount = 0;
    uint32_t arrSize = 0;
    uint32_t findCount = 0;
    scanf("%u %u %u", &etCount, &arrSize, &findCount);

    struct hashT* myHT = newHT(100003);

    for (uint32_t curEt = 0; curEt < etCount; curEt++) {
        uint32_t* etArr = (uint32_t*)calloc(arrSize, sizeof(uint32_t));

        for (uint32_t curElem = 0; curElem < arrSize; curElem++) {
            scanf("%u", etArr + curElem);
        }

        fast_sort(etArr, etArr + arrSize);
        insertHT(myHT, etArr, arrSize);
    }

    uint32_t* findArr = malloc(arrSize * sizeof(uint32_t));
    for (uint32_t curFind = 0; curFind < findCount; curFind++) {
        for (uint32_t curElem = 0; curElem < arrSize; curElem++) {
            scanf("%u", findArr + curElem);
        }
        fast_sort(findArr, findArr + arrSize);

        if (findHT(myHT, findArr, arrSize) != -1)
            printf("1\n");
        else   
            printf("0\n");
    }

    free(findArr);
    myHT = deleteHT(myHT);
}
