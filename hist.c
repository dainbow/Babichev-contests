#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const int32_t STACK_BEGINNING_CAPACITY = 32;
const int32_t STACK_INCREASE_MULTIPLIER = 2;

struct rectangle {
    uint32_t beginInd;
    uint32_t height;
};

struct stack
{
    int8_t *data;

    uint64_t elemSize;

    uint64_t size;
    uint64_t capacity;

    void* memory;
};

struct stack *stack_new(size_t elem_size)
{
    assert(elem_size > 0);

    struct stack *newStack = (struct stack *)calloc(1, sizeof(struct stack));
    assert(newStack);

    newStack->elemSize  = elem_size;
    newStack->size      = 0;
    newStack->memory    = newStack;

    newStack->data = (int8_t *)calloc(STACK_BEGINNING_CAPACITY, elem_size);
    assert(newStack->data);

    newStack->capacity = STACK_BEGINNING_CAPACITY;

    return newStack;
}

int stack_push(struct stack *st, const void *elem) {
    if (!st || !elem)
        return 1;

    if (st->size == st->capacity) {
        int8_t *newPtr = (int8_t *)realloc(st->data,
                                           st->elemSize * st->capacity * STACK_INCREASE_MULTIPLIER);
        if (!newPtr)
            return 1;

        st->data      = newPtr;
        st->capacity *= STACK_INCREASE_MULTIPLIER;
    }

    memcpy(st->data + st->elemSize * st->size, elem, st->elemSize);
    st->size++;

    return 0;
}

int stack_pop(struct stack *st, void *elem) {
    if (!st || !elem || !st->size)
        return 1;

    memcpy(elem, st->data + st->elemSize * (--st->size), st->elemSize);

    return 0;
}

int stack_top(struct stack *st, void *elem) {
    if (!st || !elem || !st->size)
        return 1;

    memcpy(elem, st->data + st->elemSize * st->size, st->elemSize);

    return 0;
}

int stack_empty(struct stack const *st) {
    assert(st);

    return (st->size == 0);
}

struct stack *stack_delete(struct stack *st) {
    assert(st);

    st->capacity = 0;
    st->elemSize = 0;
    st->size = 0;

    free(st->data);
    free(st->memory);

    return NULL;
}

int main() {
    struct stack* newStack = stack_new(sizeof(struct rectangle));

    uint32_t amount = 0;
    scanf("%u", &amount);

    uint32_t lastX = 0;
    uint32_t curX  = 0;
    
    uint32_t curS  = 0;
    uint32_t maxS  = 0;

    struct rectangle tempRec = {};

    for (uint32_t curIdx = 0; curIdx < amount; curIdx++) {
        scanf("%u", &curX);

        if (curX > lastX) {
            for (uint32_t curDiff = lastX + 1; curDiff <= curX; curDiff++) {
                tempRec.height   = curDiff;
                tempRec.beginInd = curIdx;

                stack_push(newStack, &tempRec);
            }
        }
        else if (curX < lastX) {
            while (!stack_empty(newStack)) {
                stack_pop(newStack, &tempRec);

                if (tempRec.height > curX) {
                    curS = tempRec.height * (curIdx - tempRec.beginInd);

                    maxS = (curS > maxS) ? curS : maxS; 
                }
                else {
                    stack_push(newStack, &tempRec);

                    break;
                }
            }
        }

        lastX = curX;
    }

    while(!stack_empty(newStack)) {
        stack_pop(newStack, &tempRec);

        curS = tempRec.height * (amount - tempRec.beginInd);

        maxS = (curS > maxS) ? curS : maxS; 
    }

    printf("%u", maxS);

    newStack = stack_delete(newStack);
}
