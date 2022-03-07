#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

struct set {
    void* data;

    uint64_t capacity;
    uint64_t size;
};

struct set* set_new(size_t capacity) {
    struct set* setPtr  = calloc(1, sizeof(struct set));

    setPtr->data        = calloc(sizeof(uint64_t), (capacity / 64) + 1);
    setPtr->capacity    = capacity;

    return setPtr;
}

int set_insert(struct set* s, size_t elem) {
    if (elem >= s->capacity)
        return 1;
    
    uint64_t newElem = elem >> 6; 

    uint64_t rememberByte = *(uint64_t*)(s->data + newElem);
    *((uint64_t*)(s->data) + newElem) |= ((uint64_t)1 << (elem - (newElem << 6)));

    if (rememberByte == *((uint64_t*)(s->data) + newElem))
        return 1;

    s->size++;

    return 0;
}

int set_erase(struct set* s, size_t elem) {
    if (elem >= s->capacity)
        return 1;
    
    uint64_t newElem = elem >> 6; 

    uint64_t rememberByte = *(uint64_t*)(s->data + newElem);
    *((uint64_t*)(s->data) + newElem) &= (UINT64_MAX) - ((uint64_t)1 << (elem - (newElem << 6)));

    if (rememberByte == *((uint64_t*)(s->data) + newElem))
        return 1;

    s->size--;

    return 0;
}

int set_find(struct set const *s, size_t elem) {
    if (elem >= s->capacity)
        return 0;

    return *((uint64_t*)(s->data) + (elem / 64)) & (1 << (elem % 64));
}

struct set* set_delete(struct set* s) {
    s->capacity = 0;
    s->size     = 0;
    
    free(s->data);
    free(s);

    return NULL;
}

int set_empty(struct set const* s) {
    return (s->size != 0);
}

uint64_t fastLog (uint64_t logof) {
    for (uint64_t curDegr = 0; curDegr < 64; curDegr++) {
        if (((uint64_t)1 << curDegr) == logof)
            return curDegr;
    }
     
    return 111;
}

ssize_t set_findfirst(struct set const *s, size_t start) {
    uint64_t newStart    = start >> 6;
    uint64_t ostatok     = start - (newStart << 6); 

    uint64_t firstByteToCheck = ((*((uint64_t*)(s->data) + newStart) >> ostatok) << ostatok);
    if ((firstByteToCheck ^ (firstByteToCheck - 1)) & firstByteToCheck)
        return (ssize_t)((newStart << 6) + fastLog((firstByteToCheck ^ (firstByteToCheck - 1)) & firstByteToCheck));

    for (uint64_t* curByte = ((uint64_t*)(s->data) + newStart + 1); curByte < ((uint64_t*)s->data + (s->capacity / 64) + 1); curByte++) {
        if ((*curByte ^ (*curByte - 1)) & *curByte)
            return (ssize_t)((((curByte - ((uint64_t*)s->data + newStart)) + newStart) << 6) + fastLog((*curByte ^ (*curByte - 1)) & *curByte));
    }

    return -1;
}

size_t set_size(struct set const* s) {
    return s->size;
}

void set_print(struct set const* s) {
    printf("[");

    if (s->size) {
        uint64_t flag = 0;

        for (uint64_t curElem = 0; curElem < s->capacity; curElem++) {
            if (*((uint64_t*)(s->data) + (curElem / 64)) & ((uint64_t)1 << (curElem % 64))) {
                if (flag)
                    printf(", ");

                printf("%lu", curElem);

                if (!flag)
                    flag = 1;
            }
        }
    }

    printf("]\n");
}

int main() {
    struct set *s = set_new(10000);

    set_insert(s, 1);
    set_insert(s, 3);
    set_insert(s, 9);
    set_insert(s, 228);

    set_erase(s, 228);
    //printf("FINDING 9 %d\n", set_find(s, 9));

    set_insert(s, 500);
    //printf("FINDING FIRST AFTER 10 %ld\n", set_findfirst(s, 10));

    set_print(s);

    assert(set_delete(s) == NULL);
}
