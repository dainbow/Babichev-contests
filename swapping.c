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
    int32_t key;
    int32_t value;
};

struct hashT {
    struct pair*   data;
    enum states*   state;

    uint32_t capacity;
};

uint32_t H1(int32_t s, const uint32_t hashsize) {
    uint32_t sum = 0;
    const int32_t FACTOR = 5;

    sum += s;
    sum <<= FACTOR;

    return sum % hashsize;
}

uint32_t H2(int32_t s, uint32_t hashsize) {
    uint32_t h = 0, a = 31415, b = 27183;

    h = (a * h + s) % hashsize;
    a = a * b % (hashsize - 1);

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

int32_t findHT(struct hashT* hT, int32_t key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (hT->data[curPlace].key == key) {
                    return hT->data[curPlace].value;
                }
                break;
            }
            case EMPTY: {
                return key;
            }

            case DELETED: 
                break;
            default:
                assert(0);
        }
    }

    return 0;
}

void insertHT(struct hashT* hT, int32_t key, int32_t value) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        if ((hT->state[curPlace] != BUSY) || (key == hT->data[curPlace].key)) {
            hT->data[curPlace].key   = key;
            hT->data[curPlace].value = value;
            hT->state[curPlace] = BUSY;
            
            return;
        }
    }
}

void eraseHT(struct hashT* hT, int32_t key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (hT->data[curPlace].key == key) {
                    hT->state[curPlace] = DELETED;

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
    uint32_t amount = 0;
    scanf("%u", &amount);

    struct hashT* myHT = newHT(4000037);

    int32_t leftSwap  = 0;
    int32_t rightSwap = 0;
    for (uint32_t curSwap = 0; curSwap < amount; curSwap++) {
        scanf("%d %d", &leftSwap, &rightSwap);

        int32_t leftIdx  = findHT(myHT, leftSwap);
        int32_t rightIdx = findHT(myHT, rightSwap);

        printf("%d\n", ((rightIdx - leftIdx) > 0) ? (rightIdx - leftIdx) : (leftIdx - rightIdx));

        insertHT(myHT, leftSwap, rightIdx);
        insertHT(myHT, rightSwap, leftIdx);
    }

    myHT = deleteHT(myHT);
}

