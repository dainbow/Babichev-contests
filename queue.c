#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

const uint16_t QUEUE_BEGINNING_SIZE      = (uint16_t) UINT16_MAX;

struct queue {
    int8_t* data;

    uint64_t size;
    uint64_t capacity;
    uint64_t elemSize;

    uint16_t head;
    uint16_t tail;
};

struct queue* queue_new(size_t elem_size) {
    assert(elem_size);

    struct queue* newQ = calloc(1, sizeof(struct queue) + 1);

    newQ->data = (int8_t*)calloc(QUEUE_BEGINNING_SIZE + 1, elem_size);
    assert(newQ->data);

    newQ->elemSize  = elem_size;
    newQ->capacity  = QUEUE_BEGINNING_SIZE + 1;

    return newQ;
}

int queue_push(struct queue *q, const void *elem) {
    if (q->size == q->capacity)
        return 1;

    memcpy(q->data + q->elemSize * q->head++, elem, q->elemSize);

    q->size++;

    return 0;
}

int queue_pop (struct queue *q, void *elem) {
    if (!q->size)
        return 1;
    
    memcpy(elem, q->data + q->elemSize * q->tail++, q->elemSize);
    
    q->size--;

    return 0;
}

int queue_empty(struct queue const *q) {
    assert(q);

    return (q->size == 0);
}

struct queue *queue_delete(struct queue *q) {
    assert(q);
    
    free(q->data);
    free(q);

    return NULL;
}

void queue_print(struct queue const* q, void (*pf)(void const* data)) {
    assert(q);
    assert(pf);

    printf("[");

    if (q->size) {
        const uint16_t redHead = q->head - 1;

        for (uint16_t curElem = q->tail; curElem != redHead; curElem++) {
            pf(q->data + curElem * q->elemSize);

            printf(", ");
        }

        pf(q->data + redHead * q->elemSize);
    }

    printf("]\n");
}
