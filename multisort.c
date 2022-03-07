#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void merge(void* arr, int32_t left, int32_t middle, int32_t right, uint32_t size, int32_t (*cmp)(const void* elem1, const void* elem2, int32_t* arr, int32_t arrSize), void* hArr1, void* hArr2, int32_t* prArr) {
    int32_t arr1Size = middle + 1 - left;
    int32_t arr2Size = right - middle;

    for (int32_t curElem = 0; curElem < arr1Size; curElem++) {
        memcpy(hArr1 + curElem * size, arr + (left + curElem) * size, size);
    }

    for (int32_t curElem = 0; curElem < arr2Size; curElem++) {
        memcpy(hArr2 + curElem * size, arr + (middle + 1 + curElem) * size, size);
    }

    int32_t idx1 = 0;
    int32_t idx2 = 0;
    int32_t idxBig = left;

    while ((idx1 < arr1Size) && (idx2 < arr2Size)) {
        if (cmp(hArr1 + idx1 * size, hArr2 + idx2 * size, prArr, (size - sizeof(void*)) / sizeof(int32_t))) {
            memcpy(arr + idxBig * size, hArr1 + idx1 * size, size);
            idx1++;
        }
        else {
            memcpy(arr + idxBig * size, hArr2 + idx2 * size, size);
            idx2++;
        }

        idxBig++;
    }

    while (idx1 < arr1Size) {
        memcpy(arr + idxBig * size, hArr1 + idx1 * size, size);

        idx1++;
        idxBig++;
    }

    while (idx2 < arr2Size) {
        memcpy(arr + idxBig * size, hArr2 + idx2 * size, size);

        idx2++;
        idxBig++;
    }
}

void mergeSort(void* arr, int32_t left, int32_t right, uint32_t size, int32_t (*cmp)(const void* elem1, const void* elem2, int32_t* arr, int32_t arrSize), void* hArr1, void* hArr2, int32_t* prArr) {
    if (left < right) {
        int32_t middle = left + (right - left) / 2;

        mergeSort(arr, left, middle, size, cmp, hArr1, hArr2, prArr);
        mergeSort(arr, middle + 1, right, size, cmp, hArr1, hArr2, prArr);

        merge(arr, left, middle, right, size, cmp, hArr1, hArr2, prArr);
    }
}

int32_t arrCmp(const void* elem1, const void* elem2, int32_t* arr, int32_t arrSize) {
    for(int32_t curInt = 0; curInt < arrSize; curInt++) {
        if (*((int32_t*)(elem1 + sizeof(void*) + (arr[curInt] - 1) * sizeof(int32_t))) < *((int32_t*)(elem2 + sizeof(void*) + (arr[curInt] - 1) * sizeof(int32_t))))
            return 0;
        else if (*((int32_t*)(elem1 + sizeof(void*) + (arr[curInt] - 1) * sizeof(int32_t))) > *((int32_t*)(elem2 + sizeof(void*) + (arr[curInt] - 1) * sizeof(int32_t))))
            return 1;
    }

    return 0;
}

int main() {
    int32_t exampleAmount = 0;
    int32_t intAmount = 0;

    scanf("%d", &exampleAmount);
    scanf("%d", &intAmount);

    int32_t* priorityArr = calloc(sizeof(int32_t), intAmount);

    for (int32_t curPriorityNum = 0; curPriorityNum < intAmount; curPriorityNum++) {
        scanf("%d", priorityArr + curPriorityNum);
    }

    int8_t* intArr       = calloc(sizeof(int32_t), exampleAmount * (intAmount + 2));

    int8_t* helpArr1     = calloc(sizeof(int32_t), exampleAmount * (intAmount + 2));
    int8_t* helpArr2     = calloc(sizeof(int32_t), exampleAmount * (intAmount + 2));

    for (int32_t curExNum = 0; curExNum < exampleAmount; curExNum++) {
        *(int8_t**)(intArr + curExNum * (intAmount * sizeof(int32_t) + sizeof(void*))) = calloc(sizeof(int8_t), 40); 
    }

    for (int32_t curExNum = 0; curExNum < exampleAmount; curExNum++) {
        scanf("%s ", *(int8_t**)(intArr + curExNum * (intAmount * sizeof(int32_t) + sizeof(void*))));

        for (int32_t curInt = 0; curInt < intAmount; curInt++) {
            scanf("%d", (int32_t*)(intArr + curExNum * (intAmount * sizeof(int32_t) + sizeof(void*)) + sizeof(void*) + curInt * sizeof(int32_t)));
        }
    }
    mergeSort(intArr, 0, exampleAmount - 1, sizeof(int32_t) * intAmount + sizeof(int8_t*), arrCmp, helpArr1, helpArr2, priorityArr);

    for (int32_t curExample = 0; curExample < exampleAmount; curExample++) {
        printf("%s\n", *(int8_t**)(intArr + curExample * (intAmount * sizeof(int32_t) + sizeof(void*))));
    }

    for (int32_t curExNum = 0; curExNum < exampleAmount; curExNum++) {
        free(*(int8_t**)(intArr + curExNum * (intAmount * sizeof(int32_t) + sizeof(void*)))); 
    }

    free(helpArr2);
    free(helpArr1);
    free(intArr);
    free(priorityArr);
}
