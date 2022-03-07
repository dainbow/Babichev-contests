#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t minDiff(int32_t* array, int32_t index, int32_t curSum, int32_t* index1, int32_t* arr, const int32_t maxSum, const int32_t maxIdx);
int32_t intCmp(const void* num1, const void* num2);
int32_t binSearch(int32_t* arr, int32_t right, const int32_t value);

int main() {
    uint32_t amount = 0;
    scanf("%d", &amount);

    int32_t intArray[51] = {};
    int32_t maxSum  = 0;
    for (uint32_t curElem = 0; curElem < amount; curElem++) {
        scanf("%d", intArray + curElem);
        maxSum += intArray[curElem];
    }
    int32_t* partSumArr1     = calloc(1 << (amount >> 1), sizeof(int32_t)); 
    int32_t  partSumIdx1     = 0;

    partSumIdx1 = minDiff(intArray, 0, 0, &partSumIdx1, partSumArr1, maxSum / 2, amount / 2);
    //qsort(partSumArr1, partSumIdx1 + 1, sizeof(int32_t), intCmp);
    
    int32_t* partSumArr2     = calloc(1 << (amount - (amount >> 1)), sizeof(int32_t)); 
    int32_t partSumIdx2     = 0;

    partSumIdx2 = minDiff(intArray + amount / 2, 0, 0, &partSumIdx2, partSumArr2, maxSum / 2, amount - (amount >> 1));
    qsort(partSumArr2, partSumIdx2 + 1, sizeof(int32_t), intCmp);

    

    // for (int32_t idx = 0; idx <= partSumIdx1; idx++) {
    //     printf("%d, ", partSumArr1[idx]);
    // }
    // printf("\n");

    // for (int32_t idx = 0; idx <= partSumIdx2; idx++) {
    //     printf("%d, ", partSumArr2[idx]);
    // }
    // printf("\n");

    int32_t sumSave = maxSum;
    maxSum >>= 1;
    int32_t answer = 0;
    int32_t binRes = 0;

    for (; (partSumIdx1) >= 0; partSumIdx1--) {
        if ((binRes = (binSearch(partSumArr2, partSumIdx2, maxSum - partSumArr1[partSumIdx1]) + 
                                                                            partSumArr1[partSumIdx1])) > answer) {
            answer = binRes;
        
            if (answer == maxSum)
                break;
        }
    }

    printf("%d\n", sumSave - (answer << 1));

    free(partSumArr1);
    free(partSumArr2);
}

int32_t binSearch(int32_t* arr, int32_t right, const int32_t value) {
    int32_t left = 0; 
    int32_t maxValue = 0;
    
    while (left <= right) {
        int32_t curPlace = (right + left) / 2;
        int32_t curElem = arr[curPlace];

        if (curElem > value) {
            right = curPlace - 1;
        }
        else if (curElem == value) {
            return curElem;
        }
        else {
            left = curPlace + 1;

            maxValue = curElem;
        }
    }

    return maxValue;
} 

int32_t intCmp(const void* num1, const void* num2) {
    return *(int32_t*)num1 >= *(int32_t*)num2;
}

int32_t minDiff(int32_t* array, int32_t index, int32_t curSum, int32_t* index1, int32_t* arr, const int32_t maxSum, const int32_t maxIdx) {
    if (curSum > maxSum)
       return 0;
    
    if (index < maxIdx) {
        int32_t lBranch = minDiff(array, index + 1, curSum, index1, arr, maxSum, maxIdx);
        int32_t rBranch = minDiff(array, index + 1, curSum + array[index], index1, arr, maxSum, maxIdx);
        
        return ((lBranch - rBranch) > 0) ? lBranch : rBranch;
    }
    
    arr[*index1] = curSum;

    return (*index1)++;
}
