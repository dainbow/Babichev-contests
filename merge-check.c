#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int32_t intCmp(const void* elem1, const void* elem2) {
    return *(int64_t*)elem1 > *(int64_t*)elem2;
}

int main() {
    int64_t amount = 0;
    scanf("%d", &amount);

    int64_t* array = calloc(sizeof(int64_t), 2* amount);
    for (int64_t curElem = 0; curElem < amount * 2; curElem++) {
        scanf("%d", array + curElem);
    }

    qsort(array, 2 * amount, sizeof(int64_t), intCmp);

    int64_t answer = 0;
    for (int64_t curElem = 0; curElem < amount * 2; curElem += 2) {
        answer += array[curElem];
    }
    printf("%ld\n", answer % 1000000000);

    free(array);
}
