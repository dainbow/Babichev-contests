// Итак, суть задачи заключается в том, чтобы создать идеальный кэш.
// Так как мы умеем смотреть в будущее, то мы знаем, когда нам в следующий раз понадобится данный элемент.
// Поэтому при переполнении кэша надо выкидывать элемент, который понадобится нам в следующий раз позднее всего.
// Такая стратегия верна, т.к. полезность наличия данного элемента в кэше каждый такт уменьшается.
// То есть мы должны минимизировать время, когда каждый элемент "ожидает" своего следующего вызова и просто занимает наше место в кэше.

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <immintrin.h>

const int32_t CAPACITY = 200003;

#pragma GCC target "avx2"

struct pair {
    uint64_t value;

    uint32_t key;
    uint32_t hashIdx;
};

struct heap {
    struct pair* heapArr;

    uint64_t heapLast;
    uint64_t heapType;
};

struct heapNum {
    uint64_t heapNum;
    uint8_t isFree;
};

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct hashPair {
    uint64_t key;
    struct heap hashHeap;

    uint32_t amount;
    uint32_t hashIdx;
};

struct hashT {
    struct hashPair* data;
    char*   state;

    uint32_t capacity;
};

void createHeap(struct heap* heapArr, uint64_t heapN, uint64_t heapS, uint64_t heapT) {
    heapArr[heapN].heapArr = calloc(sizeof(struct pair), 2 * heapS + 1);

    heapArr[heapN].heapType = heapT;
    heapArr[heapN].heapLast = 0;
}

uint64_t SiftDown(struct heap* heapArr, uint64_t heapN, uint32_t elemIdx, struct hashT* HT) {
    uint32_t curChild  = elemIdx;
    uint32_t curParent = (curChild - 1) / 2;

    while ((curChild > 0) && (heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent].key < heapArr[heapN].heapArr[curChild].key) + 
                             !heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent].key > heapArr[heapN].heapArr[curChild].key))) {
        if (HT) {
            HT->data[heapArr[heapN].heapArr[curParent].hashIdx].hashIdx = curChild;
            HT->data[heapArr[heapN].heapArr[curChild].hashIdx].hashIdx  = curParent;
        }
        
        struct pair tempElem = heapArr[heapN].heapArr[curParent];
        heapArr[heapN].heapArr[curParent] = heapArr[heapN].heapArr[curChild];
        heapArr[heapN].heapArr[curChild]  = tempElem;

        curChild  = curParent;
        curParent = (curChild - 1) / 2; 
    } 

    return curChild;
}

uint64_t addHeap(struct heap* heapArr, uint64_t heapN, struct pair* newElem, struct hashT* HT) {
    heapArr[heapN].heapArr[heapArr[heapN].heapLast++] = (*newElem);

    return SiftDown(heapArr, heapN, heapArr[heapN].heapLast - 1, HT);
}

void removeTop(struct heap* heapArr, uint64_t heapN, struct hashT* HT) {
    heapArr[heapN].heapArr[0] = heapArr[heapN].heapArr[--heapArr[heapN].heapLast];

    uint32_t curElem = 0;
    while(1) {
        uint32_t childToSwap = curElem;

        if ((2 * curElem + 1) < heapArr[heapN].heapLast) {
            if (heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 1].key > heapArr[heapN].heapArr[childToSwap].key) +
               !heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 1].key < heapArr[heapN].heapArr[childToSwap].key))
                childToSwap = 2 * curElem + 1;
        }

        if ((2 * curElem + 2) < heapArr[heapN].heapLast) {
            if (heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 2].key > heapArr[heapN].heapArr[childToSwap].key) +
               !heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 2].key < heapArr[heapN].heapArr[childToSwap].key))
                childToSwap = 2 * curElem + 2;
        }

        if (childToSwap == curElem)
            break;

        if (HT) {
            HT->data[heapArr[heapN].heapArr[curElem].hashIdx].hashIdx      = childToSwap;
            HT->data[heapArr[heapN].heapArr[childToSwap].hashIdx].hashIdx  = curElem;
        }

        struct pair tempElem = heapArr[heapN].heapArr[childToSwap];
        heapArr[heapN].heapArr[childToSwap] = heapArr[heapN].heapArr[curElem];
        heapArr[heapN].heapArr[curElem] = tempElem;

        curElem = childToSwap;
    }
}

void heapDelete (struct heap* heapArr, uint64_t heapN) {
    free(heapArr[heapN].heapArr);

    heapArr[heapN].heapLast = 0;
    heapArr[heapN].heapType = 0;
}

uint32_t H1(uint64_t s, uint32_t hashsize) {
    uint64_t sum = 0;
    const int32_t FACTOR = 5;

    sum += s;
    sum <<= FACTOR;

    return sum % hashsize;
}

uint32_t H2(uint64_t s, uint32_t hashsize) {
    return _mm_crc32_u64(137, s);
}

struct hashT* newHT(uint64_t capacity) {
    struct hashT* h = calloc(1, sizeof(struct hashT));
    if (h == NULL)
        return h;

    h->data  = calloc(h->capacity = capacity, sizeof(struct hashPair));
    h->state = calloc(capacity, sizeof(h->state[0]));
    
    if ((h->data == NULL) || (h->state == NULL)) {
        free(h->state);
        free(h->data);
        free(h);

        return NULL;
    }
    
    return h;
}

int32_t findHT(struct hashT* hT, uint64_t key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (hT->data[curPlace].key == key) {
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

    return -1;
}

uint64_t insertHT(struct hashT* hT, uint64_t key, uint64_t value, char countFlag, uint64_t hashIdx) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
            if ((hT->state[curPlace] != BUSY) || ((hT->state[curPlace] == BUSY) && (key == hT->data[curPlace].key))) {
                hT->data[curPlace].key   = key;

                if (countFlag) {
                    hT->data[curPlace].amount++;
                    hT->data[curPlace].hashIdx = hashIdx;
                }
                else {
                    if (hT->data[curPlace].hashHeap.heapArr == NULL)
                        createHeap(&hT->data[curPlace].hashHeap, 0, hT->data[curPlace].amount, 0);

                    struct pair newElem = {};
                    newElem.key = value;

                    addHeap(&hT->data[curPlace].hashHeap, 0, &newElem, NULL);
                }
                

                hT->state[curPlace] = BUSY;
                
                return curPlace;
            }
    }

    return 0;
}

void eraseHT(struct hashT* hT, uint64_t key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (hT->data[curPlace].key == key) {
                    hT->state[curPlace] = DELETED;
                    
                    hT->data[curPlace].key   = 0;
                    heapDelete(&hT->data[curPlace].hashHeap, 0);
                    hT->data[curPlace].amount = 0;

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
        if (hT->data[curElem].hashHeap.heapArr)
            heapDelete(&hT->data[curElem].hashHeap, 0);
    }

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;    
}

int main() {
    uint32_t memoryAmount = 0;
    uint32_t rqstAmount   = 0;

    scanf("%u %u", &memoryAmount, &rqstAmount);

    struct heap* heapsArr      = calloc(1, sizeof(struct heap));
    createHeap(heapsArr, 0, memoryAmount + 2, 1);

    struct hashT* lastIdxHt = newHT(CAPACITY);
    struct hashT* cacheHt   = newHT(CAPACITY);

    uint64_t* rqstArr = calloc(rqstAmount, sizeof(uint64_t));

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        scanf("%lu", rqstArr + curRqst);

        insertHT(lastIdxHt, rqstArr[curRqst], curRqst, 1, 0);
    }

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        insertHT(lastIdxHt, rqstArr[curRqst], curRqst, 0, 0);
    }

    uint64_t asnwer = 0;
    uint64_t curSize = 0;

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        uint32_t findedElem = 0;

        if ((findedElem = findHT(cacheHt, rqstArr[curRqst])) == -1) {
            asnwer++;

            uint32_t findedIdx  = findHT(lastIdxHt, rqstArr[curRqst]);

            removeTop(&lastIdxHt->data[findedIdx].hashHeap, 0, NULL);

            uint64_t lastIdx = 0;
            if (lastIdxHt->data[findedIdx].hashHeap.heapLast > 0) {
                lastIdx = lastIdxHt->data[findedIdx].hashHeap.heapArr[0].key;
            }
            else {
                lastIdx = UINT64_MAX;
            }

            // printf("------------------------------------\n");
            // printf("ADDING %lu\n", rqstArr[curRqst]);

            if (curSize < memoryAmount) {
                // for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                //     printf("%lu ", heapsArr[0].heapArr[curElem].value);
                // }
                // printf("\n");

                struct pair newPair = {};
                newPair.key   = lastIdx;
                newPair.value = rqstArr[curRqst];
                newPair.hashIdx = insertHT(cacheHt, rqstArr[curRqst], lastIdx, 1, heapsArr[0].heapLast);

                addHeap(heapsArr, 0, &newPair, cacheHt);

                curSize++;

                // for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                //     printf("%lu ", heapsArr[0].heapArr[curElem].value);
                // }
                // printf("\n");
            }
            else {
                // for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                //     printf("%lu ", heapsArr[0].heapArr[curElem].value);
                // }
                // printf("\n");
                
                eraseHT(cacheHt, heapsArr[0].heapArr[0].value);
                removeTop(heapsArr, 0, cacheHt);
                
                struct pair newPair = {};
                newPair.key   = lastIdx;
                newPair.value = rqstArr[curRqst];
                newPair.hashIdx = insertHT(cacheHt, rqstArr[curRqst], lastIdx, 1, heapsArr[0].heapLast);

                addHeap(heapsArr, 0, &newPair, cacheHt);

                // for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                //     printf("%lu ", heapsArr[0].heapArr[curElem].value);
                // }
                // printf("\n");
            }
        }
        else {
            // printf("------------------------------------\n");
            // printf("%lu IN HASH\n", rqstArr[curRqst]);

            uint32_t findedIdx  = findHT(lastIdxHt, rqstArr[curRqst]);

            removeTop(&lastIdxHt->data[findedIdx].hashHeap, 0, NULL);

            uint64_t lastIdx = 0;
            if (lastIdxHt->data[findedIdx].hashHeap.heapLast > 0) {
                lastIdx = lastIdxHt->data[findedIdx].hashHeap.heapArr[0].key;
            }
            else {
                lastIdx = UINT64_MAX;
            }

            heapsArr[0].heapArr[cacheHt->data[findedElem].hashIdx].key = lastIdx;
            SiftDown(heapsArr, 0, cacheHt->data[findedElem].hashIdx, cacheHt);
        }
    }

    printf("%lu\n", asnwer);

    heapDelete(heapsArr, 0);

    free(heapsArr);
    free(rqstArr);

    cacheHt   = deleteHT(cacheHt);
    lastIdxHt = deleteHT(lastIdxHt);
}

