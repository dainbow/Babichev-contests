#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const int32_t CAPACITY = 50000;

struct pair {
    uint64_t key;
    uint64_t value;
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

void createHeap(struct heap* heapArr, uint64_t heapN, uint64_t heapS, uint64_t heapT) {
    heapArr[heapN].heapArr = calloc(sizeof(struct pair), 2 * heapS + 1);

    heapArr[heapN].heapType = heapT;
    heapArr[heapN].heapLast = 0;
}

uint64_t SiftDown(struct heap* heapArr, uint64_t heapN, uint64_t elemIdx) {
    uint64_t curChild  = elemIdx;
    uint64_t curParent = (curChild - 1) / 2;

    while ((curChild > 0) && (heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent].key < heapArr[heapN].heapArr[curChild].key) + 
                             !heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent].key > heapArr[heapN].heapArr[curChild].key))) {
        struct pair tempElem = heapArr[heapN].heapArr[curParent];
        heapArr[heapN].heapArr[curParent] = heapArr[heapN].heapArr[curChild];
        heapArr[heapN].heapArr[curChild]  = tempElem;

        curChild  = curParent;
        curParent = (curChild - 1) / 2; 
    } 

    return curChild;
}

uint64_t addHeap(struct heap* heapArr, uint64_t heapN, struct pair newElem) {
    heapArr[heapN].heapArr[heapArr[heapN].heapLast++] = newElem;

    return SiftDown(heapArr, heapN, heapArr[heapN].heapLast - 1);
}

void removeTop(struct heap* heapArr, uint64_t heapN) {
    heapArr[heapN].heapArr[0] = heapArr[heapN].heapArr[--heapArr[heapN].heapLast];

    uint64_t curElem = 0;
    while(1) {
        uint64_t childToSwap = curElem;

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

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct trio {
    uint64_t key;
    uint64_t value;
    uint64_t amount;
};

struct hashT {
    struct trio* data;
    char*   state;

    uint32_t capacity;
};

uint32_t H1(uint64_t s, uint32_t hashsize) {
    uint64_t sum = 0;
    const int32_t FACTOR = 5;

    sum += s;
    sum <<= FACTOR;

    return sum % hashsize;
}

uint32_t H2(uint64_t s, uint32_t hashsize) {
    uint64_t h = 0, a = 31415, b = 27183;

    h = (a * h + s) % hashsize;
    a = a * b % (hashsize - 1);

    return h;
}

struct hashT* newHT(uint64_t capacity) {
    struct hashT* h = calloc(1, sizeof(struct hashT));
    if (h == NULL)
        return h;

    h->data  = calloc(h->capacity = capacity, sizeof(struct trio));
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

void insertHT(struct hashT* hT, uint64_t key, uint64_t value) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
            if ((hT->state[curPlace] != BUSY) || ((hT->state[curPlace] == BUSY) && (key == hT->data[curPlace].key))) {
                hT->data[curPlace].key   = key;
                hT->data[curPlace].value = value;
                hT->data[curPlace].amount++;

                hT->state[curPlace] = BUSY;
                
                return;
            }
    }
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
                    hT->data[curPlace].value = 0;
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

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;    
}

int main() {
    uint32_t memoryAmount = 0;
    uint32_t rqstAmount   = 0;

    struct hashT* lastIdxHt = newHT(CAPACITY);
    struct hashT* cacheHt   = newHT(CAPACITY);

    struct heap* heapsArr      = calloc(2, sizeof(struct heap));
    createHeap(heapsArr, 0, memoryAmount + 1, 0);
    createHeap(heapsArr, 1, memoryAmount + 1, 0);

    scanf("%u %u", &memoryAmount, &rqstAmount);

    uint64_t* rqstArr = calloc(rqstAmount, sizeof(uint64_t));

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        scanf("%lu", rqstArr + curRqst);

        insertHT(lastIdxHt, rqstArr[curRqst], curRqst);
    }

    uint64_t asnwer = 0;
    uint64_t curSize = 0;

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        uint32_t findedElem = 0;

        while ((heapsArr[0].heapLast > 0) && (heapsArr[0].heapArr[0].key <= curRqst)) {
            addHeap(heapsArr, 1, heapsArr[0].heapArr[0]);
            removeTop(heapsArr, 0);
        }

        if ((findedElem = findHT(cacheHt, rqstArr[curRqst])) == -1) {
            asnwer++;
            uint64_t lastIdx    = lastIdxHt->data[findHT(lastIdxHt, rqstArr[curRqst])].value;
            // uint64_t elemAmount = lastIdxHt->data[findHT(lastIdxHt, rqstArr[curRqst])].amount;

            printf("------------------------------------\n");
            printf("ADDING %lu\n", rqstArr[curRqst]);

            if (curSize < memoryAmount) {
                for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                    printf("%lu ", heapsArr[0].heapArr[curElem].value);
                }
                printf("\n");

                struct pair newPair = {};
                newPair.key   = lastIdx;
                newPair.value = rqstArr[curRqst];

                insertHT(cacheHt, rqstArr[curRqst], lastIdx);

                if ((newPair.key < (curRqst + memoryAmount)) && (newPair.key > curRqst))
                    addHeap(heapsArr, 0, newPair);
                else {
                    addHeap(heapsArr, 1, newPair);
                }

                curSize++;

                for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                    printf("%lu ", heapsArr[0].heapArr[curElem].value);
                }
                printf("\n");
            }
            else {
                for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                    printf("%lu ", heapsArr[0].heapArr[curElem].value);
                }
                printf("\n");

                if (heapsArr[1].heapLast > 0) {
                    eraseHT(cacheHt, heapsArr[1].heapArr[0].value);
                    removeTop(heapsArr, 1);
                }
                else {
                    eraseHT(cacheHt, heapsArr[0].heapArr[0].value);
                    removeTop(heapsArr, 0);
                }

                struct pair newPair = {};
                newPair.key   = lastIdx;
                newPair.value = rqstArr[curRqst];

                insertHT(cacheHt, rqstArr[curRqst], lastIdx);
                addHeap(heapsArr, 0, newPair);

                for (uint32_t curElem = 0; curElem < heapsArr[0].heapLast; curElem++) {
                    printf("%lu ", heapsArr[0].heapArr[curElem].value);
                }
                printf("\n");
            }
        }
        else {
            printf("------------------------------------\n");
            printf("%lu IN HASH\n", rqstArr[curRqst]);
        }
    }

    printf("%lu\n", asnwer);

    heapDelete(heapsArr, 0);

    free(heapsArr);
    free(rqstArr);

    cacheHt   = deleteHT(cacheHt);
    lastIdxHt = deleteHT(lastIdxHt);
}

