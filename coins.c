#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct partSum {
    uint32_t value;

    uint32_t coinsAmount;
};

int32_t minDiff(int32_t* array, int32_t index, int32_t curSum, int32_t curCoins,
                                                            int32_t* index1, struct partSum* arr, int32_t* minCoins,
                                                                    const int32_t maxSum, const int32_t maxIdx);
int32_t partCmp(const void* num1, const void* num2);
int32_t binSearch(struct partSum* arr, int32_t right, const int32_t value);

int main() {
    int32_t price  = 0;
    int32_t amount = 0;
    scanf("%d %d", &price, &amount);

    amount <<= 1;
    int32_t intArray[31] = {};
    int32_t maxSum  = 0;
    for (uint32_t curElem = 0; curElem < amount; curElem += 2) {
        scanf("%d", intArray + curElem);
        intArray[curElem + 1] = intArray[curElem];

        maxSum += (intArray[curElem] << 1);
    }

    if (maxSum < price) {
        printf("Impossible");
        
        return 0;
    }

    if (maxSum == price) {
        printf("%d", amount);

        return 0;
    }

    int32_t minCoins = INT32_MAX;

    struct partSum* partSumArr1     = calloc(1 << (amount >> 1), sizeof(struct partSum)); 
    int32_t         partSumIdx1     = 0;

    partSumIdx1 = minDiff(intArray, 0, 0, 0, &partSumIdx1, partSumArr1, &minCoins, price, amount / 2);
    
    struct partSum* partSumArr2     = calloc(1 << (amount - (amount >> 1)), sizeof(struct partSum)); 
    int32_t partSumIdx2             = 0;

    partSumIdx2 = minDiff(intArray + (amount / 2), 0, 0, 0, &partSumIdx2, partSumArr2, &minCoins, price, amount - (amount >> 1));
    qsort(partSumArr2, partSumIdx2 + 1, sizeof(struct partSum), partCmp);
    

    if (minCoins != INT32_MAX) {
        printf("%d", minCoins);
        return 0;
    }

    int32_t binRes = 0;

    for (; (partSumIdx1) >= 0; partSumIdx1--) {
        if ((binRes = binSearch(partSumArr2, partSumIdx2, price - partSumArr1[partSumIdx1].value))) {
            if ((binRes + partSumArr1[partSumIdx1].coinsAmount) < minCoins) {
                // printf("%d + %d\n", partSumArr1[partSumIdx1].value, price - partSumArr1[partSumIdx1].value);

                minCoins = binRes + partSumArr1[partSumIdx1].coinsAmount;
            }
        }
    }

    if (minCoins != INT32_MAX) {
        printf("%d", minCoins);
        return 0;
    }

    printf("Change");

    free(partSumArr1);
    free(partSumArr2);
}

int32_t binSearch(struct partSum* arr, int32_t right, const int32_t value) {
    int32_t left = 0; 
    
    while (left <= right) {
        int32_t curPlace = (right + left) / 2;
        struct partSum curElem = arr[curPlace];

        if (curElem.value > value) {
            right = curPlace - 1;
        }
        else if (curElem.value == value) {
            return curElem.coinsAmount;
        }
        else {
            left = curPlace + 1;
        }
    }

    return 0;
} 

int32_t partCmp(const void* num1, const void* num2) {
    return ((struct partSum*)num1)->value >= ((struct partSum*)num2)->value;
}

int32_t minDiff(int32_t* array, int32_t index, int32_t curSum, int32_t curCoins,
                                                            int32_t* index1, struct partSum* arr, int32_t* minCoins,
                                                                    const int32_t maxSum, const int32_t maxIdx) {
    if (curSum > maxSum)
       return 0;

    if (curSum == maxSum) {
        if (curCoins < *minCoins)
            *minCoins = curCoins;

        return 0;
    }
    
    if (index < maxIdx) {
        int32_t lBranch = minDiff(array, index + 1, curSum, curCoins, index1, arr, minCoins, maxSum, maxIdx);
        int32_t rBranch = minDiff(array, index + 1, curSum + array[index], curCoins + 1, index1, arr, minCoins, maxSum, maxIdx);
        
        return ((lBranch - rBranch) > 0) ? lBranch : rBranch;
    }
    
    arr[*index1].value       = curSum;
    arr[*index1].coinsAmount = curCoins;

    return (*index1)++;
}
