#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#define NDEBUG
#pragma GCC optimize ("O3")
const uint32_t QUEUE_BEGINNING_SIZE      = 1 << 21;
const uint32_t MASK = QUEUE_BEGINNING_SIZE - 1;
struct queue {

    uint64_t capacity;
    uint64_t elemSize;

    // uint32_t mask;
    uint64_t head;
    uint64_t tail;
    
    uint64_t size;
    void* data;
};

struct queue* queue_new(size_t elem_size) {
    assert(elem_size);

    struct queue* newQ = calloc(1, sizeof(struct queue));

    newQ->data = calloc((QUEUE_BEGINNING_SIZE), elem_size);
    assert(newQ->data);

    newQ->elemSize  = elem_size;
    newQ->capacity  = QUEUE_BEGINNING_SIZE;
    // newQ->mask      = newQ->capacity - 2;

    return newQ;
}

int queue_push(struct queue *q, const void *elem) {
    // if (q->size == q->capacity) return 1;
    //     int8_t* newPtr = realloc(q->data, q->capacity * q->elemSize * 2);

    //     //if (!newPtr) {
    //     //    return 1;
    //     //}

    //     q->data = newPtr;
    // }

    memcpy(q->data + q->elemSize * q->head++, elem, q->elemSize);

    q->size++;
    // if(q->head == MASK) abort();
    // q->head &= MASK;//q->mask;
    
    return 0;
}

int queue_pop (struct queue *q, void *elem) {
    if (!q->size)
        return 1;
    
    

    memcpy(elem, q->data + q->elemSize * q->tail++, q->elemSize);
    
    q->size--;
    // q->tail &= MASK;

    return 0;
}

int queue_empty(struct queue const *q) {
    assert(q);

    return (q->size == 0);
}

struct queue *queue_delete(struct queue *q) {
    free(q->data);
    free(q);

    return NULL;
}

void queue_print(struct queue const* q, void (*pf)(void const* data)) {
    // assert(q);
    // assert(pf);
    
    printf("[");

    if (q->size) {
        void* ptr = q->data + q->tail * q->elemSize;
        for(size_t i = 0; i < q->size - 1; ++i, ptr += q->elemSize){
            pf(ptr);
            printf(", ");
        }
        /*
        const uint32_t redHead = (q->head - 1) & (MASK);

        for (uint32_t curElem = q->tail; curElem != redHead; curElem = (curElem + 1) & (MASK)) {
            pf(q->data + curElem * q->elemSize);

            printf(", ");
        }
        */
        pf(q->data + ((q->tail + q->size - 1) /*& (MASK)*/) * q->elemSize);
    }

    printf("]\n");
}

#ifdef LOCAL
            
static void print_double(void const *st) {
   printf("%lf", *(double *)st);
}

int main() {
    struct queue *q = queue_new(sizeof (double));
    for (int i = 0; i < 10; i++) {
        double tmp = i * i;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);
    while (!queue_empty(q)) {
        double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }                                                                                                                                                                                        
 
    q = queue_delete(q);
}

#endif