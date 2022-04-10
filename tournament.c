#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct pair {
    char* key;

    int32_t value;
    uint32_t time;
};

struct hashT {
    struct pair* data;
    char*   state;

    uint32_t capacity;
};

uint32_t H1(char* s, uint32_t hashsize) {
    uint32_t sum = 0;
    const int32_t FACTOR = 5;

    while (*s) {
        sum <<= FACTOR;
        sum += *s++;
    }

    return sum % hashsize;
}

uint32_t H2(char* s, uint32_t hashsize) {
    uint32_t h = 0, a = 31415, b = 27183;
    while(*s) {
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

    for (uint64_t curSpace = 0; curSpace < capacity; curSpace++) {
        h->data[curSpace].time  = UINT32_MAX;
        h->data[curSpace].value = INT32_MIN;
    }
    
    if ((h->data == NULL) || (h->state == NULL)) {
        free(h->state);
        free(h->data);
        free(h);

        return NULL;
    }
    
    return h;
}

int32_t findHT(struct hashT* hT, char* key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity) % hT->capacity, increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (strcmp(hT->data[curPlace].key, key) == 0) {
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

void insertHT(struct hashT* hT, char* key, uint32_t value, uint32_t time) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity) % hT->capacity, increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
          if (hT->state[curPlace] != BUSY) {
              hT->data[curPlace].key   = key;
              hT->data[curPlace].value = value;
              hT->data[curPlace].time  = time;

              hT->state[curPlace] = BUSY;
              
              return;
          }
    }
}

void eraseHT(struct hashT* hT, char* key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity) % hT->capacity, increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (strcmp(hT->data[curPlace].key, key) == 0) {
                    hT->state[curPlace] = DELETED;
                    free(hT->data[curPlace].key);
                    hT->data[curPlace].key = NULL;

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

int32_t pairCmp(const void* elem1, const void* elem2) {
    if (((struct pair*)elem1)->value > ((struct pair*)elem2)->value) {
        return -1;
    }
    else if (((struct pair*)elem1)->value < ((struct pair*)elem2)->value) {
        return 1;
    }
    else {
        return ((struct pair*)elem1)->time > ((struct pair*)elem2)->time;
    }
}

int main() {
    uint32_t attmpAmount   = 0;
    uint32_t peopleAmount  = 0;

    scanf("%u %u", &peopleAmount, &attmpAmount);
    struct hashT* myHT = newHT((29 * peopleAmount) / 10);

    for (uint32_t curAttemp = 0; curAttemp < attmpAmount; curAttemp++) {
        char* personName = calloc(45, sizeof(char));
        assert(personName);

        uint32_t points  = 0;

        scanf("%45s %u", personName, &points);

        int32_t idx = 0;
        if ((idx = findHT(myHT, personName)) != -1) {
            if (points > myHT->data[idx].value) {
                myHT->data[idx].value = points;
                myHT->data[idx].time  = curAttemp;
            }
            
            free(personName);
        }
        else {
            insertHT(myHT, personName, points, curAttemp);
        }
    }

    qsort(myHT->data, (29 * peopleAmount) / 10, sizeof(struct pair), pairCmp);

    for (uint32_t curPerson = 0; myHT->data[curPerson].key; curPerson++) {
        printf("%s\n", myHT->data[curPerson].key);
    }

    myHT = deleteHT(myHT);
}