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
    uint64_t heapsAmount = 0;

    scanf("%lu\n", &heapsAmount);
    struct heap* heapsArr      = calloc(heapsAmount, sizeof(struct heap));

    struct heapNum* heapNumConverter = calloc(heapsAmount, sizeof(struct heapNum));
    for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
        heapNumConverter[curNum].isFree = 1;
    }

    int64_t commandType = 0;
    while ((commandType = getchar()) != EOF) {
        switch (commandType)
        {
        case '0': {
            uint64_t heapNumber = 0;
            uint64_t heapSize   = 0;
            uint64_t heapType   = 0;

            scanf("%lu %lu %lu", &heapNumber, &heapSize, &heapType);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if (heapNumConverter[curNum].isFree) {
                    heapNumConverter[curNum].heapNum = heapNumber;
                    heapNumConverter[curNum].isFree  = 0;
                }
            }

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            createHeap(heapsArr, heapNumber, heapSize, heapType);
            break;
        }
        case '1': {
            uint64_t heapNumber = 0;
            int64_t heapAdd    = 0;

            scanf("%lu %ld", &heapNumber, &heapAdd);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            addHeap(heapsArr, heapNumber, heapAdd);
            break;
        }
        case '2': {
            uint64_t heapNumber = 0;

            scanf("%lu", &heapNumber);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            if (heapsArr[heapNumber].heapLast)
                printf("%ld\n", heapsArr[heapNumber].heapArr[0]);
            break;
        }
        case '3': {
            uint64_t heapNumber = 0;

            scanf("%lu", &heapNumber);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            removeTop(heapsArr, heapNumber);
            break;
        }
        case '4': {
            uint64_t heapNumber = 0;
            
            scanf("%lu", &heapNumber);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            printf("%lu\n", heapsArr[heapNumber].heapLast);
            break;
        }
        case '5': {
            uint64_t heapNum1 = 0;
            uint64_t heapNum2 = 0;

            scanf("%lu %lu", &heapNum1, &heapNum2);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNum1 == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNum1 = curNum; 
                    break;
                }
            }

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNum2 == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNum2 = curNum; 
                    break;
                }
            }

            heapMove(heapsArr, heapNum1, heapNum2);
            break;
        }
        case '6': {
            uint64_t heapNumber = 0;

            scanf("%lu", &heapNumber);

            for (uint32_t curNum = 0; curNum < heapsAmount; curNum++) {
                if ((heapNumber == heapNumConverter[curNum].heapNum) && (heapNumConverter[curNum].isFree == 0)) {
                    heapNumber = curNum; 
                    break;
                }
            }

            heapNumConverter[heapNumber].isFree  = 1;
            heapNumConverter[heapNumber].heapNum = 0;

            heapDelete(heapsArr, heapNumber);
            break;
        }
        
        default:
            break;
        }
    }

    free(heapsArr);
    free(heapNumConverter);
}

