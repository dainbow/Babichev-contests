#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

struct vector {
    int8_t* data;

    uint64_t elemSize;

    uint64_t capacity;
    uint64_t size;

    void* memory;
};

struct uintFromSet {
    uint32_t value;

    uint8_t setNum;
};

struct vector* vector_new(size_t elems, size_t elem_size) {
    struct vector* newVector = (struct vector*)malloc(sizeof(struct vector));
    
    newVector->elemSize = elem_size;
    newVector->size     = elems;

    newVector->data = (int8_t*)malloc(elems * elem_size);
    
    newVector->capacity = elems;
    newVector->memory = newVector;

    return newVector;
}

int vector_increase(struct vector *v, size_t new_size) {
    v->data = (int8_t*)realloc(v->data, new_size * v->elemSize);
    v->capacity = new_size;

    return 0;
}

// int vector_resize(struct vector *v, size_t new_size) {
//     v->size = new_size;

//     if (2 * new_size >= v->capacity)
//         vector_increase(v, 2 * new_size);

//     return 0;
// }

int vector_push(struct vector *v, void const *elem) {
    if (v->size == v->capacity) {
        vector_increase(v, 2 * v->capacity + 1);
    }

    memcpy(v->data + v->elemSize * v->size++, elem, v->elemSize);

    return 0;
}

// int vector_pop (struct vector *v, void *elem) {
//     if (v->size == 0)
//         return 1;
    
//     memcpy(elem, v->data + v->elemSize * --v->size, v->elemSize);

//     if (v->size >= v->capacity / 4)
//         vector_resize(v, v->size);

//     return 0;
// }

// int vector_empty(struct vector const *st) {
//     return (st->size == 0);
// } 

struct vector *vector_delete(struct vector *v) {
    v->elemSize = 0;
    v->size     = 0;
    v->capacity = 0;

    free(v->data);
    free(v->memory);

    return NULL;
}

// void vector_print(struct vector const *v, void (*pf)(void const *data)) {
//     printf("[");

//     if (v->size) {
//         for (uint64_t curElem = 0; curElem < v->size - 1; curElem++) {
//             pf(v->data + curElem * v->elemSize);
//             printf(", ");
//         }

//         pf(v->data + v->elemSize * (v->size - 1));
//     }

//     printf("]\n");
// }

// int vector_set(struct vector *v, size_t index, void const *elem) {
//     if (!(index < v->capacity))
//         return 1;

//     memcpy(v->data + index * v->elemSize, elem, v->elemSize);

//     return 0;
// }

int vector_get(struct vector const *v, size_t index, void *elem) {
    if (!(index < v->capacity))
        return  1;

    memcpy(elem, v->data + index * v->elemSize, v->elemSize);

    return 0;
}

// size_t vector_size(struct vector const *v) {
//     return v->size;
// }

int32_t structCmp(const void* num1, const void* num2) {
    return ((struct uintFromSet*)num1)->value > ((struct uintFromSet*)num2)->value;
}

// static void uintPrint(void const* data) {
//     printf("%u", ((struct uintFromSet*)data)->value);
// }

int main() {
    struct vector* setA = vector_new(0, sizeof(struct uintFromSet));
    struct vector* setB = vector_new(0, sizeof(struct uintFromSet));
    struct vector* setU = vector_new(0, sizeof(struct uintFromSet));
    struct vector* setI = vector_new(0, sizeof(struct uintFromSet));

    struct uintFromSet tempStruct;

    tempStruct.value  = 1;
    tempStruct.setNum = 0;

    while (tempStruct.value != 0) {
        scanf("%u", &tempStruct.value);

        vector_push(setA, &tempStruct);
        vector_push(setU, &tempStruct);
    }

    tempStruct.value  = 1;
    tempStruct.setNum = 1;
    while (tempStruct.value != 0) {
        scanf("%u", &(tempStruct.value));

        vector_push(setB, &tempStruct);
        vector_push(setU, &tempStruct);
    }

    qsort(setU->data, setU->size, sizeof(struct uintFromSet), structCmp);
    qsort(setA->data, setA->size, sizeof(struct uintFromSet), structCmp);
    qsort(setB->data, setB->size, sizeof(struct uintFromSet), structCmp);

    uint32_t* answerArr = calloc(sizeof(uint32_t), setU->size);
    uint64_t answLength = 0;

    // vector_print(setA, uintPrint);
    // vector_print(setB, uintPrint);
    // vector_print(setU, uintPrint);

    for (uint64_t curElem = 0; curElem < setU->size; curElem++) {
        struct uintFromSet tempStruct1 = {};
        struct uintFromSet tempStruct2 = {};
        
        vector_get(setU, curElem, &tempStruct1);

        uint64_t left  = 0;
        uint64_t right = 0;

        if (tempStruct1.setNum == 0)
            right = setB->size - 1;
        else 
            right = setA->size - 1;

        
        answerArr[answLength++] = tempStruct1.value;

        while (left <= right) {
            if (tempStruct1.setNum == 0)
                vector_get(setB, (left + right) / 2, &tempStruct2);
            else
                vector_get(setA, (left + right) / 2, &tempStruct2);


            if (tempStruct2.value == tempStruct1.value) {
                answerArr[answLength--] = 0; 

                break;
            }
            else if (tempStruct2.value > tempStruct1.value) {
                right = (((left + right) / 2) != 0) ? (((left + right) / 2) - 1) : 0;
            }
            else {
                left = ((left + right) / 2) + 1;
            }
        }
    }

    for (uint64_t curElem = 0; curElem < answLength; curElem++) {
        printf("%u ", answerArr[curElem]);
    }

    setU = vector_delete(setU);
    setA = vector_delete(setA);
    setB = vector_delete(setB);
    setI = vector_delete(setI);
    free(answerArr);
}
