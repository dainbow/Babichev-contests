#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <immintrin.h>

#define CAPACITY 18903
#pragma GCC target "avx2"

enum states {
    EMPTY = 0,
    DELETED,
    BUSY,
};

struct coords {
    uint16_t x;
    uint16_t y;
};

struct pair {
    struct coords key;

    struct pair* next;
    char value;
};

struct hashT {
    struct pair* data;
    char* state;

    struct pair* head;
    uint32_t capacity;
};

uint64_t H1(struct coords s, const uint32_t hashsize) {
    uint32_t hash = 0;

    char* x = (char*)&s.x;
    char* y = (char*)&s.y;

    hash = _mm_crc32_u8(hash, x[0]);
    hash = _mm_crc32_u8(hash, x[1]);

    hash = _mm_crc32_u8(hash, y[0]);
    hash = _mm_crc32_u8(hash, y[1]);

    return hash % hashsize;
}

uint64_t H2(struct coords s, const uint32_t hashsize) {
    const uint64_t a = 937;
    const uint64_t b = 1201;

    return (((uint64_t)s.x * a) + ((uint64_t)s.y * b));
}

int32_t coordsCmp(struct coords elem1, struct coords elem2) {
    return (elem1.x == elem2.x) && (elem1.y == elem2.y);
}

struct hashT *newHT(uint64_t capacity) {
    struct hashT *h = calloc(1, sizeof(struct hashT));
    if (h == NULL)
        return h;

    h->data  = calloc(h->capacity = capacity, sizeof(h->data[0]));
    h->state = calloc(capacity, sizeof(h->state[0]));

    if ((h->data == NULL) || (h->state == NULL))
    {
        free(h->state);
        free(h->data);
        free(h);

        return NULL;
    }

    return h;
}

int32_t findHT(struct hashT *hT, struct coords key)
{
    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity) , tries = 0;
         tries != hT->capacity;
         curPlace = (curPlace + increment) % hT->capacity, tries++)
    {
        switch (hT->state[curPlace])
        {
        case BUSY:
        {
            if (coordsCmp(hT->data[curPlace].key, key))
            {
                return curPlace;
            }
            break;
        }
        case EMPTY:
        {
            return -1;
        }

        case DELETED:
        default:
            break;
        }
    }

    return 0;
}

uint8_t neibourghCountAndUpdate(struct hashT *hT, struct coords key, char isUpdate) {
    uint8_t answer = 0;
    struct coords neibourgh = {};
    int32_t idx = 0;

    for (uint8_t curState = 0; curState < 8; curState++) {
        if (curState & 0x4) {
            neibourgh.x = key.x + 1 * (curState & 0x1) - 1 * (1 - (curState & 0x1));
            neibourgh.y = key.y + 1 * ((curState & 0x2) >> 1) - 1 * (1 - ((curState & 0x2) >> 1));
        }
        else {
            if (((curState & 0x2) >> 1) ^ (curState & 0x1)) {
                neibourgh.x = key.x + 1 * (curState & 0x1);
                neibourgh.y = key.y + 1 * ((curState & 0x2) >> 1);
            }
            else {
                neibourgh.x = key.x - 1 * (curState & 0x1);
                neibourgh.y = key.y - 1 * (1 - (curState & 0x1));   
            }
        }

        idx = findHT(hT, neibourgh);
        if (idx != -1) {
            hT->data[idx].value += (1 * isUpdate);
            answer++;
        }
    }

    return answer;
}

void insertHT(struct hashT *hT, struct coords key, const uint32_t width, const uint32_t height) {
    for (uint64_t curPlace = H1(key, hT->capacity), increment = H2(key, hT->capacity), tries = 0;
         tries != hT->capacity;
         curPlace = (curPlace + increment) % hT->capacity, tries++)
    {
        if (hT->state[curPlace] != BUSY)
        {
            hT->data[curPlace].key = key;
            hT->data[curPlace].value = neibourghCountAndUpdate(hT, key, 1);
            hT->state[curPlace] = BUSY;

            hT->data[curPlace].next = hT->head;
            hT->head = hT->data + curPlace;

            return;
        }
        else if (coordsCmp(hT->data[curPlace].key, key))
            return;
    }
}

struct hashT *deleteHT(struct hashT *hT)
{
    if (hT == NULL)
        return NULL;

    free(hT->state);
    free(hT->data);
    free(hT);
    return NULL;
}

void qProcess(struct coords* q, uint32_t* qSize, struct coords crd, struct hashT* hT, const uint32_t width, const uint32_t height) {
    if ((crd.x < width) && (crd.y < height)) {
        int32_t idx = findHT(hT, crd);
        if (idx != -1) {
            if ((hT->data[idx].value == 2) || (hT->data[idx].value == 3)) {
                q[*qSize] = crd;
                (*qSize)++;
            }
        }
        else {
            if (neibourghCountAndUpdate(hT, crd, 0) == 3) {
                q[*qSize] = crd;
                (*qSize)++;
            }
        }
    }
}

void addToQ(struct coords* q, uint32_t* qSize, struct coords key, struct hashT* hT, const uint32_t width, const uint32_t height) {
    struct coords neibourgh = {};

    for (uint8_t curState = 0; curState < 8; curState++) {
        if (curState & 0x4) {
            neibourgh.x = key.x + 1 * (curState & 0x1) - 1 * (1 - (curState & 0x1));
            neibourgh.y = key.y + 1 * ((curState & 0x2) >> 1) - 1 * (1 - ((curState & 0x2) >> 1));
        }
        else {
            if (((curState & 0x2) >> 1) ^ (curState & 0x1)) {
                neibourgh.x = key.x + 1 * (curState & 0x1);
                neibourgh.y = key.y + 1 * ((curState & 0x2) >> 1);
            }
            else {
                neibourgh.x = key.x - 1 * (curState & 0x1);
                neibourgh.y = key.y - 1 * (1 - (curState & 0x1));   
            }
        }

        qProcess(q, qSize, neibourgh, hT, width, height);
    }
}

int main() {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t aliveAmount = 0;
    uint32_t tactAmount = 0;

    scanf("%u %u %u %u", &width, &height, &aliveAmount, &tactAmount);
    struct hashT *myHT = newHT(CAPACITY);
    for (uint32_t curCell = 0; curCell < aliveAmount; curCell++) {
        struct coords nowCoords = {};
        scanf("%hu %hu", &nowCoords.x, &nowCoords.y);

        insertHT(myHT, nowCoords, width, height);
    }

    struct coords* q = calloc(CAPACITY, sizeof(q[0])); 
    uint32_t qSize = 0;
    for (uint32_t curTact = 0; curTact < tactAmount; curTact++) {
        for (struct pair* curPair = myHT->head; curPair; curPair = curPair->next) {
            if ((curPair->value == 2) || (curPair->value == 3)) {
                q[qSize] = curPair->key;
                qSize++;
            }
            
            addToQ(q, &qSize, curPair->key, myHT, width, height);
        }

        for (struct pair* curPair = myHT->head; curPair; curPair = curPair->next) {
            myHT->state[curPair - myHT->data] = EMPTY;
            
            curPair->key.x = UINT16_MAX;
            curPair->key.y = UINT16_MAX;
            curPair->value = 0;
        }
        myHT->head = NULL;

        for (uint32_t curCell = 0; curCell < qSize; curCell++) {
            insertHT(myHT, q[curCell], width, height);
        }
        qSize = 0;
    }

    for (uint32_t curWidth = 0; curWidth < width; curWidth++)  {
        for (uint32_t curHeight = 0; curHeight < height; curHeight++) {
            struct coords nowCoords = {curWidth, curHeight};

            if (findHT(myHT, nowCoords) != -1) 
                printf("%u %u\n", curWidth, curHeight);
        }
    }

    free(q);
    myHT = deleteHT(myHT);
}
