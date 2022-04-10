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

struct node {
    uint16_t arrNum;
    struct node* next;
};

struct pair {
    int32_t key;
    struct node* value;
};

struct hashT {
    struct pair* data;
    char*   state;

    uint32_t capacity;
};

uint32_t H1(int32_t s, const uint32_t hashsize) {
    uint32_t sum = 0;
    const uint32_t FACTOR = 5;
    
    sum <<= FACTOR;
    sum += s;

    return sum % hashsize;
}

uint32_t H2(int32_t s, const uint32_t hashsize) {
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

void insertHT(struct hashT* hT, int32_t key, uint16_t value, uint16_t** arr, uint16_t* maxNum) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
            if ((hT->state[curPlace] != BUSY) || (key == hT->data[curPlace].key)) {
                hT->data[curPlace].key   = key;
                hT->state[curPlace]      = BUSY;
                
                struct node* newNode = calloc(1, sizeof(struct node));
                newNode->arrNum = value;

                for (struct node* curNode = hT->data[curPlace].value; curNode; curNode = curNode->next) {
                    arr[value][curNode->arrNum]++;
                    if (arr[value][curNode->arrNum] > *maxNum) {
                        (*maxNum)++;
                    }
                }

                newNode->next   = hT->data[curPlace].value;
                hT->data[curPlace].value = newNode;
            
                return;
            }
    }
}

struct hashT* deleteHT(struct hashT* hT) {
    if (hT == NULL)
        return NULL;

    for (uint32_t curElem = 0; curElem < hT->capacity; curElem++) {
        if (hT->state[curElem] == BUSY) {
            for (struct node* curNode = hT->data[curElem].value; curNode;) {
                struct node* nextNode = curNode->next;
                free(curNode);

                curNode = nextNode;
            }
        }
    }

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;    
}

int main() {
    uint16_t arrAmount = 0;
    uint32_t arrSize   = 0;
    scanf("%hu %u", &arrAmount, &arrSize);

    struct hashT* myHT = newHT(1000003);

    uint16_t** arr = calloc(arrAmount, sizeof(uint16_t*));
    for (uint32_t curArr = 0; curArr < arrAmount; curArr++) {
        arr[curArr] = calloc(arrAmount - 1, sizeof(uint16_t));
    }

    int32_t tempKey = 0;
    uint16_t answer = 0;
    for (uint16_t curArr = 0; curArr < arrAmount; curArr++) {
        for (uint32_t curElem = 0; curElem < arrSize; curElem++) {
            scanf("%d", &tempKey);

            insertHT(myHT, tempKey, curArr, arr, &answer);
        }
    }

    printf("%hu\n", answer);
    myHT = deleteHT(myHT);
    for (uint32_t curArr = 0; curArr < arrAmount; curArr++) {
        free(arr[curArr]);
    }
    free(arr);
}
