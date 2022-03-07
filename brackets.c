#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const int32_t STACK_BEGINNING_CAPACITY = 32;
const int32_t STACK_INCREASE_MULTIPLIER = 2;

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

int stack_top(struct stack *st, void *elem)
{
    if (!st || !elem || !st->size)
        return 1;

    memcpy(elem, st->data + st->elemSize * st->size, st->elemSize);

    return 0;
}

int stack_empty(struct stack const *st)
{
    assert(st);

    return (st->size == 0);
}

struct stack *stack_delete(struct stack *st)
{
    assert(st);

    st->capacity = 0;
    st->elemSize = 0;
    st->size = 0;

    free(st->data);
    free(st->memory);

    return NULL;
}

int main() {
    struct stack* newStack = stack_new(1);

    uint8_t curChar   = 0;
    uint8_t popedChar = 0;

    uint8_t badFlag   = 0;
    while((curChar = getchar()) != '\n') {
        switch(curChar) {
            case '{':
            case '(':
            case '<':
            case '[':
                stack_push(newStack, &curChar);
                break;
            case '}':
                badFlag |= stack_pop(newStack, &popedChar);
                
                badFlag |= (popedChar != '{');
                break;
            case ')':
                badFlag |= stack_pop(newStack, &popedChar);
                
                badFlag |= (popedChar != '(');
                break;
            case '>':
                badFlag |= stack_pop(newStack, &popedChar);
                
                badFlag |= (popedChar != '<');
                break;
            case ']':
                badFlag |= stack_pop(newStack, &popedChar);
                
                badFlag |= (popedChar != '[');
                break;
        }
    }

    badFlag |= (!stack_empty(newStack));

    if (badFlag)
        printf("NO");
    else
        printf("YES");

    newStack = stack_delete(newStack);
}   
