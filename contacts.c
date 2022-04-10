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

int main() {
    uint32_t cmdNum = 0;
    scanf("%u", &cmdNum);

    struct hashT* myHT = newHT(10000);

    for (uint32_t curCmd = 0; curCmd < cmdNum; curCmd++) {
        int32_t curChar = getchar();

        switch (curChar) {
            case 'A': {
                char* key   = calloc(35, sizeof(char));
                char* value = calloc(35, sizeof(char));

                scanf("DD %35s %35s", key, value);

                int32_t idx = 0;
                if ((idx = findHT(myHT, key)) != -1) {
                    printf("ERROR\n");

                    free(key);
                    free(value);
                }
                else {
                    insertHT(myHT, key, value);
                }
            }
            break;

            case 'P': {
                char* key = calloc(35, sizeof(char));

                scanf("RINT %35s", key);

                int32_t idx = 0;
                if ((idx = findHT(myHT, key)) != -1) {
                    printf("%s %s\n", myHT->data[idx].key, myHT->data[idx].value);
                }
                else {
                    printf("ERROR\n");
                }
            }
            break;

            case 'E': {
                char* key   = calloc(35, sizeof(char));
                char* value = calloc(35, sizeof(char));

                scanf("DITPHONE %35s %35s", key, value);

                int32_t idx = 0;
                if ((idx = findHT(myHT, key)) != -1) {
                    free(myHT->data[idx].value);
                    free(key);
                    
                    myHT->data[idx].value = value;
                }
                else {
                    printf("ERROR\n");
                }
            }
            break;

            case 'D': {
                char* key   = calloc(35, sizeof(char));

                scanf("ELETE %35s", key);

                int32_t idx = 0;
                if ((idx = findHT(myHT, key)) != -1) {
                    free(key);
                    free(myHT->data[idx].value);
                    free(myHT->data[idx].key);

                    myHT->state[idx] = DELETED;
                }
                else {
                    printf("ERROR\n");
                }
            }
            break;

            case '\n': 
                curCmd--;
                break;

            default:
                break;
        }
    }

    myHT = deleteHT(myHT);
}