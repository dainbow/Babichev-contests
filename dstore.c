#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct heap {
    int64_t* heapArr;

    uint64_t heapLast;
    uint64_t heapType;
};

struct heapNum {
    uint64_t heapNum;
    uint8_t isFree;
};

void createHeap(struct heap* heapArr, uint64_t heapN, uint64_t heapS, uint64_t heapT) {
    heapArr[heapN].heapArr = calloc(sizeof(int64_t), 2 * heapS + 1);

    heapArr[heapN].heapType = heapT;
    heapArr[heapN].heapLast = 0;
}

void pushDown(struct heap* heapArr, uint64_t heapN, int64_t newElem) {
    
}
 
void addHeap(struct heap* heapArr, uint64_t heapN, int64_t newElem) {
    heapArr[heapN].heapArr[heapArr[heapN].heapLast++] = newElem;

    uint64_t curChild  = heapArr[heapN].heapLast - 1;
    uint64_t curParent = (curChild - 1) / 2;

    while ((curChild > 0) && (heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent] < heapArr[heapN].heapArr[curChild]) + 
                             !heapArr[heapN].heapType * (heapArr[heapN].heapArr[curParent] > heapArr[heapN].heapArr[curChild]))) {
        int64_t tempElem = heapArr[heapN].heapArr[curParent];
        heapArr[heapN].heapArr[curParent] = heapArr[heapN].heapArr[curChild];
        heapArr[heapN].heapArr[curChild]  = tempElem;

        curChild  = curParent;
        curParent = (curChild - 1) / 2; 
    } 
}

void removeTop(struct heap* heapArr, uint64_t heapN) {
    heapArr[heapN].heapArr[0]--;

    uint64_t curElem = 0;
    while(1) {
        uint64_t childToSwap = curElem;

        if ((2 * curElem + 1) < heapArr[heapN].heapLast) {
            if (heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 1] > heapArr[heapN].heapArr[childToSwap]) +
               !heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 1] < heapArr[heapN].heapArr[childToSwap]))
                childToSwap = 2 * curElem + 1;
        }

        if ((2 * curElem + 2) < heapArr[heapN].heapLast) {
            if (heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 2] > heapArr[heapN].heapArr[childToSwap]) +
               !heapArr[heapN].heapType * (heapArr[heapN].heapArr[2 * curElem + 2] < heapArr[heapN].heapArr[childToSwap]))
                childToSwap = 2 * curElem + 2;
        }

        if (childToSwap == curElem)
            break;

        int64_t tempElem = heapArr[heapN].heapArr[childToSwap];
        heapArr[heapN].heapArr[childToSwap] = heapArr[heapN].heapArr[curElem];
        heapArr[heapN].heapArr[curElem] = tempElem;

        curElem = childToSwap;
    }
}

void heapMove (struct heap* heapArr, uint64_t heapDest, uint64_t heapSource) {
    for (uint64_t curHeapElem = 0; curHeapElem < heapArr[heapSource].heapLast; curHeapElem++)
        addHeap(heapArr, heapDest, heapArr[heapSource].heapArr[curHeapElem]);
    
    heapArr[heapSource].heapLast = 0;
}

void heapDelete (struct heap* heapArr, uint64_t heapN) {
    free(heapArr[heapN].heapArr);

    heapArr[heapN].heapLast = 0;
    heapArr[heapN].heapType = 0;
}

int main() {
    uint32_t amount = 0;
    scanf("%u", &amount);

    struct heap* heapsArr      = calloc(1, sizeof(struct heap));
    createHeap(heapsArr, 1, amount, 1);

    uint32_t min = UINT32_MAX;
    uint64_t sum = 0;
    uint32_t tempVal = 0;
    for (uint32_t curSize = 0; curSize < amount; curSize++) {
        scanf("%u", &tempVal);
        sum += tempVal;
        min = (tempVal < min) ? tempVal : min;

        addHeap(heapsArr, 1, tempVal);
    }
 
    uint32_t takenAmount = 0;
    while (sum > 1) {
        if (min == takenAmount) {
            min = UINT32_MAX;
            
            for (uint32_t curElem = 0; curElem < 2 * amount + 1; curElem++) {
                min = 
            }
        }
    }
}