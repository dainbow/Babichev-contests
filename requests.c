#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int32_t maxValue;
    int32_t xorValue;
} Elem;

uint64_t RoundSize(uint64_t size) {
    for (uint64_t curPower = 0; curPower < 65; curPower++) {
        if ((1 << curPower) >= size) {
            return (1 << curPower); 
        }
    }

    return 3;
}

void BuildDO(Elem* arr, uint64_t size, uint64_t index, int32_t* max, int32_t* xor) {
    if (index >= size) {
        *max = arr[index].maxValue;
        *xor = arr[index].xorValue;

        return;
    }

    int32_t curMax1 = 0;
    int32_t curXor1 = 0;
    BuildDO(arr, size, 2 * index, &curMax1, &curXor1);

    int32_t curMax2 = 0;
    int32_t curXor2 = 0;
    BuildDO(arr, size, 2 * index + 1, &curMax2, &curXor2);

    arr[index].maxValue = (curMax1 > curMax2) ? curMax1 : curMax2;
    arr[index].xorValue = curXor1 ^ curXor2;

    *max = arr[index].maxValue;
    *xor = arr[index].xorValue;
}

void ChangeValue(Elem* arr, uint64_t size, uint64_t index, int32_t newValue) {
    index += size;
    
    arr[index].maxValue = newValue;
    arr[index].xorValue = newValue;

    index >>= 1;

    while (index) {
        arr[index].maxValue = (arr[2 * index].maxValue > arr[2 * index + 1].maxValue) ? arr[2 * index].maxValue : arr[2 * index + 1].maxValue; 
        arr[index].xorValue = arr[2 * index].xorValue ^ arr[2 * index + 1].xorValue;

        index >>= 1;
    }
}

int32_t GetElem(Elem* arr, uint64_t index, uint64_t left, uint64_t right, uint64_t curLeft, uint64_t curRight, uint8_t type) {
    int32_t retValue = type ? INT32_MIN : 0;

    if ((left <= curLeft) && (right >= curRight)) {
        if (type == 1) {
            return (arr[index].maxValue > retValue) ? arr[index].maxValue : retValue;
        }
        else {
            return arr[index].xorValue ^ retValue;
        }
    }

    uint64_t curMiddle = (curLeft + curRight) / 2;

    if (left <= curMiddle) {
        int32_t leftRecurs = GetElem(arr, 2 * index, left, (curMiddle < right) ? curMiddle : right, curLeft, curMiddle, type);
        
        if (type == 1) {
            retValue = (leftRecurs > retValue) ? leftRecurs : retValue;
        }
        else {
            retValue = leftRecurs ^ retValue;  
        }
    }

    curMiddle++;
    if (right >= curMiddle) {
        int32_t rightRecurs = GetElem(arr, 2 * index + 1, (left > curMiddle) ? left : curMiddle, right, curMiddle, curRight, type);
       
        if (type == 1) {
            retValue = (rightRecurs > retValue) ? rightRecurs : retValue;
        }
        else {
            retValue = rightRecurs ^ retValue;  
        }
    }

    return retValue;
}   


int main() {
    uint64_t arrSize       = 0;
    uint64_t requestAmount = 0;

    scanf("%lu %lu", &arrSize, &requestAmount);
    uint64_t roundedArrSize = RoundSize(arrSize);

    Elem* arr = calloc(2 * roundedArrSize, sizeof(Elem));
    for (uint64_t curElem = 0; curElem < arrSize; curElem++) {
        scanf("%d", &arr[roundedArrSize + curElem].maxValue);

        arr[roundedArrSize + curElem].xorValue = arr[roundedArrSize + curElem].maxValue; 
    }

    int32_t trash1 = 0;
    int32_t trash2 = 0;
    BuildDO(arr, roundedArrSize, 1, &trash1, &trash2);

    for (uint64_t curReq = 0; curReq < requestAmount; curReq++) {
        uint64_t reqType = 0;
        scanf("%lu", &reqType);

        switch (reqType)
        {
        case 1: {
            uint64_t left  = 0;
            uint64_t right = 0;

            scanf("%lu %lu", &left, &right);
            
            printf("%d\n", GetElem(arr, 1, left, right, 0, roundedArrSize - 1, 0));
        }
        break;
        
        case 2: {
            uint64_t left  = 0;
            uint64_t right = 0;

            scanf("%lu %lu", &left, &right);
            
            printf("%d\n", GetElem(arr, 1, left, right, 0, roundedArrSize - 1, 1));
        }
        break;

        case 3: {
            uint64_t index   = 0;
            int32_t newValue = 0;

            scanf("%lu %d", &index, &newValue);
            ChangeValue(arr, roundedArrSize, index, newValue);
        }
        break;

        default:
            break;
        }
    }

    free(arr);
}
