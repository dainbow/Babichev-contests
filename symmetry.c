#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <immintrin.h>


#pragma GCC target "avx2"

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct coords {
    int64_t x;
    int64_t y;
    int64_t z;
};

struct pair {
    struct coords key;
    uint32_t amount;
};

struct hashT {
    struct pair* data;
    char*   state;

    uint32_t capacity;
};

uint32_t H1(struct coords* s, uint32_t hashsize) {
    uint32_t hash = 0;

    char* x = (char*)&s->x;
    char* y = (char*)&s->y;
    char* z = (char*)&s->z;

    for (uint32_t curByte = 0; curByte < 8; curByte++) {
        hash = _mm_crc32_u8(hash, x[curByte]);
        hash = _mm_crc32_u8(hash, y[curByte]);
        hash = _mm_crc32_u8(hash, z[curByte]);
    }

    return hash % hashsize;
}

uint32_t H2(struct coords* s, uint32_t hashsize) {
    uint32_t hash = 0;

    char* x = (char*)&s->x;
    char* y = (char*)&s->y;
    char* z = (char*)&s->z;

    for (uint32_t curByte = 0; curByte < 8; curByte++) {
        hash = _mm_crc32_u8(hash, y[curByte]);
        hash = _mm_crc32_u8(hash, z[curByte]);
        hash = _mm_crc32_u8(hash, x[curByte]);
    }

    return hash;
}

int32_t structCmp(struct coords* elem1, struct coords* elem2) {
    return (elem1->x == elem2->x) && (elem1->y == elem2->y) && (elem1->z == elem2->z);
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

int32_t findHT(struct hashT* hT, struct coords* key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity) % hT->capacity, increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (structCmp(&hT->data[curPlace].key, key)) {
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

void insertHT(struct hashT* hT, struct coords* key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        if (hT->state[curPlace] != BUSY) {
            hT->data[curPlace].key = *key;
            hT->state[curPlace]    = BUSY;

            return;
        }
    }
}

int32_t eraseHT(struct hashT* hT, struct coords* key) {
    assert(hT != NULL);

    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0; 
      tries != hT->capacity; 
      curPlace = (curPlace + increment) % hT->capacity, tries++) {
        switch (hT->state[curPlace]) {
            case BUSY: {
                if (structCmp(&hT->data[curPlace].key, key)) {
                    hT->state[curPlace] = DELETED;
                    return 1;
                }
                break;
            }
            case EMPTY: {
                return 0;
            }

            case DELETED: 
                break;
            default:
                assert(0);
        }
    }

    return 0;
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
    uint32_t pointsAmount;

    scanf("%u", &pointsAmount);
    struct hashT* myHT  = newHT(4 * pointsAmount);

    __int128_t sum[3] = {};

    for (uint32_t curPoint = 0; curPoint < pointsAmount; curPoint++) {
        struct coords curCords = {};
        scanf("%ld %ld %ld", &curCords.x, &curCords.y, &curCords.z);

        sum[0] += curCords.x;
        sum[1] += curCords.y;
        sum[2] += curCords.z;
        insertHT(myHT, &curCords);
    }

    sum[0] = (2 * sum[0]) / pointsAmount;
    sum[1] = (2 * sum[1]) / pointsAmount;
    sum[2] = (2 * sum[2]) / pointsAmount;
    for (uint32_t curElem = 0; curElem < myHT->capacity; curElem++) {
        if (myHT->state[curElem] == BUSY) {
            if ((sum[0] != 2 * myHT->data[curElem].key.x) ||
                (sum[1] != 2 * myHT->data[curElem].key.y) ||
                (sum[2] != 2 * myHT->data[curElem].key.z)) {
            
                struct coords simCords = {};
                simCords.x = sum[0] - myHT->data[curElem].key.x;
                simCords.y = sum[1] - myHT->data[curElem].key.y;
                simCords.z = sum[2] - myHT->data[curElem].key.z;

                myHT->state[curElem] = DELETED;
                if (eraseHT(myHT, &simCords) == 0) {
                    printf("No\n");

                    myHT = deleteHT(myHT);
                    exit(0);
                }
            }
        }
    }

    printf("Yes\n");
    // printf("%lf %lf %lf\n", sim[0], sim[1], sim[2]);

    myHT = deleteHT(myHT);
}
