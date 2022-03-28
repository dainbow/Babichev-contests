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
    char* key;
    char* value;
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

void insertHT(struct hashT* hT, char* key, char* value) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity) % hT->capacity, increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
          if (hT->state[curPlace] != BUSY) {
              hT->data[curPlace].key   = key;
              hT->data[curPlace].value = value;
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
                    free(hT->data[curPlace].value);

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
            free(hT->data[curElem].value);
        }
    }

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;    
}

int32_t main() {
    struct hashT** hTs = calloc(10, sizeof(struct hashT*));

    for (int curChar = getchar(); curChar != EOF; curChar = getchar()) {
        switch (curChar) {
            case 'c': {
                int32_t htNum       = 0;
                scanf("reate %d", &htNum);

                hTs[htNum] = newHT(CAPACITY);
            }
            break;

            case 'i': {
                int32_t htNum       = 0;
                
                char* key   = calloc(20, sizeof(char));
                char* value = calloc(20, sizeof(char));
                scanf("nsert %d %20s %20s", &htNum, key, value);
                
                if (hTs[htNum] != NULL) {
                    int32_t idx = 0;
                    if ((idx = findHT(hTs[htNum], key)) != -1) {
                        free(key);
                        free(hTs[htNum]->data[idx].value);
                        hTs[htNum]->data[idx].value = value;
                    }
                    else {
                        insertHT(hTs[htNum], key, value);
                    }
                }
                else {
                    free(key);
                    free(value);
                }
            } 
            break;
            
            case 'e': {
                int32_t htNum       = 0;
                
                char* key   = calloc(20, sizeof(char));
                scanf("rase %d %20s", &htNum, key);

                if (hTs[htNum] != NULL) {
                    eraseHT(hTs[htNum], key);
                }
                
                free(key);
            }
            break;

            case 'f': {
                int32_t htNum       = 0;
                
                char* key   = calloc(20, sizeof(char));
                scanf("ind %d %20s", &htNum, key);

                if (hTs[htNum] != NULL) {
                    int32_t idx = 0;
                    if ((idx = findHT(hTs[htNum], key)) != -1) {
                        printf("%s\n", hTs[htNum]->data[idx].value);
                    }
                    else {
                        printf("MISSING\n");
                    }
                }
                else {
                    printf("MISSING\n");
                }

                free(key);
            }
            break;

            case 'd': {
                int32_t htNum       = 0;
                scanf("estroy %d", &htNum);

                hTs[htNum] = deleteHT(hTs[htNum]);
            }
            break;

            default:
                continue; 
        }
    }

    for (uint32_t curHT = 0; curHT < 10; curHT++) {
        if (hTs[curHT] != NULL) {
            hTs[curHT] = deleteHT(hTs[curHT]);
        }
    }

    free(hTs);
}