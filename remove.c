#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

    newVector->data = (int8_t*)calloc(elems, elem_size);
    
    newVector->capacity = elems;
    newVector->memory = newVector;

    return newVector;
}

int vector_increase(struct vector *v, size_t new_size) {
    v->data = (int8_t*)realloc(v->data, new_size * v->elemSize);
    v->capacity = new_size;

    return 0;
}

int vector_resize(struct vector *v, size_t new_size) {
    v->size = new_size;

    if (2 * new_size >= v->capacity)
        vector_increase(v, 2 * new_size);

    return 0;
}

int vector_push(struct vector *v, void const *elem) {
    if (v->size == v->capacity) {
        vector_increase(v, 2 * v->capacity + 1);
    }

    memcpy(v->data + v->elemSize * v->size++, elem, v->elemSize);

    return 0;
}

int vector_pop (struct vector *v, void *elem) {
    if (v->size == 0)
        return 1;
    
    memcpy(elem, v->data + v->elemSize * --v->size, v->elemSize);

    if (v->size >= v->capacity / 4)
        vector_resize(v, v->size);

    return 0;
}

int vector_empty(struct vector const *st) {
    return (st->size == 0);
} 

struct vector *vector_delete(struct vector *v) {
    v->elemSize = 0;
    v->size     = 0;
    v->capacity = 0;

    free(v->data);
    free(v->memory);

    return NULL;
}

void vector_print(struct vector const *v, void (*pf)(void const *data)) {
    printf("[");

    if (v->size) {
        for (uint64_t curElem = 0; curElem < v->size - 1; curElem++) {
            pf(v->data + curElem * v->elemSize);
            printf(", ");
        }

        pf(v->data + v->elemSize * (v->size - 1));
    }

    printf("]\n");
}

int vector_set(struct vector *v, size_t index, void const *elem) {
    if (!(index < v->capacity))
        return 1;

    memcpy(v->data + index * v->elemSize, elem, v->elemSize);

    return 0;
}

int vector_get(struct vector const *v, size_t index, void *elem) {
    if (!(index < v->capacity))
        return  1;

    memcpy(elem, v->data + index * v->elemSize, v->elemSize);

    return 0;
}

size_t vector_size(struct vector const *v) {
    return v->size;
}

int main() {
    struct vector* newVector = vector_new(0, sizeof(int8_t));

    int8_t curChar  = 0;
    int8_t tempChar = 0;

    uint8_t inSString = 0;
    uint8_t inDString = 0;
    uint8_t inComment = 0;
    
    while((curChar = getchar()) != EOF) {
        switch(curChar) {
            case '\'':
                if (inComment == 1) {
                    inComment = 0;

                    tempChar = '/';
                    vector_push(newVector, &tempChar);
                }
                else if (inComment == 3) {
                    inComment = 2;
                }

                if ((inComment != 4) && (inComment != 2) && (!inDString)) {
                    inSString ^= 1;
                    vector_push(newVector, &curChar);
                }
                else if ((inComment != 2) && (inComment != 4)) {
                    vector_push(newVector, &curChar);
                }
                break;
            case '\"':
                if (inComment == 1) {
                    inComment = 0;

                    tempChar = '/';
                    vector_push(newVector, &tempChar);
                }
                else if (inComment == 3) {
                    inComment = 2;
                }

                if ((inComment != 4) && (inComment != 2) && (!inSString)) {
                    inDString ^= 1;
                    vector_push(newVector, &curChar);
                }
                else if ((inComment != 2) && (inComment != 4)) {
                    vector_push(newVector, &curChar);
                }
                break;
            case '/':
                if ((!inSString) && (!inDString) && (inComment == 0)) {
                    inComment = 1;
                }
                else if ((!inSString) && (!inDString) && (inComment == 3)) {
                    inComment = 0;
                }
                else if ((!inSString) && (!inDString) && (inComment == 1)) {
                    inComment = 4;

                    tempChar = '\n';
                    vector_push(newVector, &tempChar);
                }
                else if ((inComment != 2) && (inComment != 4)) {
                    vector_push(newVector, &curChar);
                }
                break;
            case '*':
                if (inComment == 3)
                    inComment = 2;

                if ((!inSString) && (!inDString) && (inComment == 1)) {
                    inComment = 2;

                    tempChar = ' ';
                    vector_push(newVector, &tempChar);
                }
                else if ((!inSString) && (!inDString) && (inComment == 2)) {
                    inComment = 3;
                } 
                else if ((inComment != 2) && (inComment != 4)) {
                    vector_push(newVector, &curChar);
                }
                break;
            case '\n':
                if (inComment == 1) {
                    inComment = 0;
                
                    tempChar = '/';
                    vector_push(newVector, &tempChar);
                }
                else if (inComment == 3)
                    inComment = 2;

                if ((!inSString) && (!inDString) && (inComment == 4)) {
                    inComment = 0;
                }
                else if ((!inSString) && (!inDString) && (inComment == 0)) {
                    tempChar = '\n';
                    vector_push(newVector, &tempChar);
                }
                break;
            default:
                if (inComment == 1) {
                    inComment = 0;
                
                    tempChar = '/';
                    vector_push(newVector, &tempChar);
                }
                else if (inComment == 3)
                    inComment = 2;

                if ((inComment != 2) && (inComment != 4)) {
                    vector_push(newVector, &curChar);
                }
        }
    }
    curChar = '\0';
    vector_push(newVector, &curChar);

    printf("%s\n", newVector->data);
    newVector = vector_delete(newVector);
}
