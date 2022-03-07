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

int vector_push(struct vector *v, void const *elem) {
    if (v->size == v->capacity) {
        vector_increase(v, 2 * v->capacity + 1);
    }

    memcpy(v->data + v->elemSize * v->size++, elem, v->elemSize);

    return 0;
}

struct vector *vector_delete(struct vector *v) {
    v->elemSize = 0;
    v->size     = 0;
    v->capacity = 0;

    free(v->data);
    free(v->memory);

    return NULL;
}

struct letter {
    int8_t ch;
    uint32_t freq;
};

int32_t letterCmp(const void* elem1, const void* elem2) {
    if (((struct letter*)elem1)->freq > ((struct letter*)elem2)->freq) {
        return -1;
    }
    else if (((struct letter*)elem1)->freq == ((struct letter*)elem2)->freq) {
        return ((struct letter*)elem1)->ch > ((struct letter*)elem2)->ch;
    }
    else {
        return 1;
    }
}

int main() {
    struct vector* string = vector_new(0, 1);

    int8_t curChar = 0;
    while((curChar = getchar()) != '\n') {
        vector_push(string, &curChar);
    }
    curChar = '\0';
    vector_push(string, &curChar);

    struct letter alpha[26] = {};

    for (uint32_t curLetter = 0; curLetter < 26; curLetter++) {
        alpha[curLetter].ch = 'A' + curLetter;
    }

    for (uint32_t curLetter = 0; string->data[curLetter] != '\0'; curLetter++) {
        alpha[string->data[curLetter] - 'A'].freq++;
    }

    qsort(alpha, 26, sizeof(struct letter), letterCmp);

    for (uint32_t curLetter = 0; alpha[curLetter].freq; curLetter++) {
        printf("%c %d\n", alpha[curLetter].ch, alpha[curLetter].freq);
    }

    vector_delete(string);
}