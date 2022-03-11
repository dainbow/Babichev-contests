#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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
    heapArr[heapN].heapArr[0] = heapArr[heapN].heapArr[--heapArr[heapN].heapLast];

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
    struct heap* heapsArr = calloc(2, sizeof(struct heap));

    uint64_t nAmount = 0;

    scanf("%lu", &nAmount);

    createHeap(heapsArr, 0, 1000000, 0);
    createHeap(heapsArr, 1, 1000000, 1);

    int64_t tempValue = 0;
    for (uint32_t curNumber = 0; curNumber < nAmount; curNumber++) {
        scanf("%ld", &tempValue);

        if (tempValue == 0) {
            scanf("%ld", &tempValue);

            if (heapsArr[0].heapLast) {
                if (tempValue > heapsArr[0].heapArr[0]) {
                    addHeap(heapsArr, 0, tempValue);
                }
                else {
                    addHeap(heapsArr, 1, tempValue);
                }
            }
            else {
                addHeap(heapsArr, 1, tempValue);
            }

            while (heapsArr[1].heapLast < heapsArr[0].heapLast) {
                addHeap(heapsArr, 1, heapsArr[0].heapArr[0]);
                removeTop(heapsArr, 0);
            }

            if ((heapsArr[1].heapLast - heapsArr[0].heapLast) == 2) {
                addHeap(heapsArr, 0, heapsArr[1].heapArr[0]);
                removeTop(heapsArr, 1);
            }
        }
        else {
            printf("%ld\n", heapsArr[1].heapArr[0]);
        }
    }

    heapDelete(heapsArr, 0);
    heapDelete(heapsArr, 1);
    free(heapsArr);
}   
