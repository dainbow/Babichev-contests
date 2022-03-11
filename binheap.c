#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct binNode {
    int32_t value;

    struct binNode* parent;
    struct binNode* next;
    struct binNode* lChild;

    uint32_t idx;
    uint32_t degree;
} node;

struct idxInfo {
    struct binNode* elem;
    uint32_t heapNum;
    uint8_t isFree;
};

node* mergeHeaps (node* heap1, node* heap2) {
    if (heap1 == NULL)
        return heap2;

    if (heap2 == NULL)
        return heap1;

    node* head = NULL;
    node* rememberHead = NULL;
    if (heap1->degree < heap2->degree) {
        head = heap1;
        rememberHead = head;
        
        heap1 = heap1->next;
        head->next = heap2;
        heap2 = heap2->next;

        head = head->next;
    }
    else {
        head = heap2;
        rememberHead = head;

        heap2 = heap2->next;
        head->next = heap1;
        heap1 = heap1->next;

        head = head->next;
    }
    while (heap1 && heap2) {
        if (heap1->degree < heap2->degree) {
            head->next = heap1;
            heap1 = heap1->next;
        }
        else {
            head->next = heap2;
            heap2 = heap2->next;
        }
    }

    while (heap1) {
        head->next = heap1;

        heap1 = heap1->next;
        head  = head->next;
    }

    while (heap2) {
        head->next = heap2;

        heap2 = heap2->next;
        head  = head->next;
    }

    return rememberHead;
}

node* unionHeap(node* heap) {
    if (heap->next == NULL)
        return heap;

    node* rememberHead = heap;

    node* prevHeap = 0;
    node* curHeap  = heap;
    node* nextHeap = heap->next;

    for (;;) {
        if (curHeap->degree == nextHeap->degree) {
            if (nextHeap->next) {
                if (nextHeap->next->degree == curHeap->degree) {
                    prevHeap = curHeap;
                    curHeap  = nextHeap;
                    nextHeap = nextHeap->next;
                }
                else {
                    if (curHeap->value < nextHeap->value) {
                        curHeap->next    = nextHeap->next;
                        nextHeap->next   = curHeap->lChild;

                        nextHeap->parent = curHeap;
                        curHeap->degree++;

                        curHeap->lChild = nextHeap;

                        nextHeap = curHeap->next;
                    }
                    else {
                        curHeap->next   = nextHeap->lChild;
                        nextHeap->lChild = curHeap;

                        curHeap->parent = nextHeap;
                        nextHeap->degree++;

                        if (prevHeap) {
                            prevHeap->next = nextHeap;
                        }

                        if (curHeap == rememberHead)
                            rememberHead = nextHeap;

                        curHeap  = nextHeap;
                        nextHeap = curHeap->next;
                    }
                }
            }
            else {
                if (curHeap->value < nextHeap->value) {
                    curHeap->next    = nextHeap->next;
                    nextHeap->next   = curHeap->lChild;

                    nextHeap->parent = curHeap;
                    curHeap->degree++;

                    curHeap->lChild = nextHeap;
                }
                else {
                    curHeap->next   = nextHeap->lChild;
                    nextHeap->lChild = curHeap;

                    curHeap->parent = nextHeap;
                    nextHeap->degree++;

                    if (prevHeap) {
                        prevHeap->next = nextHeap;
                    }

                    if (curHeap == rememberHead)
                        rememberHead = nextHeap;
                }

                break;
            }
        }
        else {
            prevHeap = curHeap;
            curHeap  = nextHeap;
            nextHeap = nextHeap->next;

            if (nextHeap == NULL)
                break;
        }
    }

    return rememberHead;
}

node* binInsert(node* head, int32_t value, node** newElemAdr, uint32_t idx) {
    *newElemAdr = calloc(1, sizeof(node));

    (*newElemAdr)->value  = value;
    (*newElemAdr)->idx    = idx;

    return unionHeap(mergeHeaps(head, *newElemAdr));
}

node* extractMin(node* heap, node** minValueAdr, uint8_t deleteFlag) {
    node* rememberHead = heap;
    node* prevNode     = NULL;

    int32_t minValue = heap->value;
    node* minNode = heap;
    node* minPrev = NULL;


    while(heap) {
        if (heap->value < minValue) {
            minNode = heap;
            minPrev = prevNode;

            minValue = heap->value;
        }
        else if (heap->value == minValue) {
            if (heap->idx < minNode->idx) {
                minNode = heap;
                minPrev = prevNode;
            }
        }

        prevNode = heap;
        heap = heap->next;
    }

    if (minValueAdr)
        *minValueAdr = minNode;

    if (deleteFlag == 0)
        return rememberHead;

    if (minPrev) {
        minPrev->next = minNode->next;
    }

    node* child = minNode->lChild;

    if (minNode == rememberHead)
        rememberHead = minNode->next;

    if (child) {
        while (child) {
            node* rememberNext = child->next;
            child->next   = NULL;
            child->parent = NULL;

            rememberHead = unionHeap(mergeHeaps(rememberHead, child));
            child = rememberNext;
        }
    }

    return rememberHead;
}

void changeValue (node* elemToChange, int32_t value, struct idxInfo* idxArr) {
    if (value < elemToChange->value) {
        elemToChange->value = value;
        while (elemToChange->parent && (elemToChange->parent->value > elemToChange->value)) {
            int32_t tempValue           = elemToChange->value;
            elemToChange->value         = elemToChange->parent->value;
            elemToChange->parent->value = tempValue;

            idxArr[elemToChange->parent->idx].elem = elemToChange;
            idxArr[elemToChange->idx].elem = elemToChange->parent;

            uint32_t tempIdx            = elemToChange->idx;
            elemToChange->idx           = elemToChange->parent->idx;
            elemToChange->parent->idx   = tempIdx;

            elemToChange = elemToChange->parent;
        }   
    }

    if (value > elemToChange->value) {
        elemToChange->value = value;

        for (;;) {
            node* child = elemToChange->lChild;
            
            node* minChild = NULL;
            int32_t minValue = INT32_MAX;

            while (child) {
                if (child->value < minValue) {
                    minValue = child->value;
                    minChild = child;
                }

                child = child->next;
            }          

            if (minValue > elemToChange->value)
                break;
            else {
                minChild->value     = elemToChange->value;
                elemToChange->value = minValue;

                idxArr[minChild->idx].elem = elemToChange;
                idxArr[elemToChange->idx].elem = minChild;

                uint32_t tempIdx    = minChild->idx;
                minChild->idx       = elemToChange->idx;
                elemToChange->idx   = tempIdx;

                elemToChange = minChild;
            }
        }
    }
}

node* printMin (node* heap) {
    node* minValue = NULL;
    node* returnV  = extractMin(heap, &minValue, 0);

    printf("%d\n", minValue->value);
    return returnV;
}

node* deleteElem (node* heap, node* elemToDel, struct idxInfo* idxArr) {
    changeValue(elemToDel, INT32_MIN, idxArr);
    node* retValue = extractMin(heap, NULL, 1);

    idxArr[elemToDel->idx].isFree = 1;
    free(elemToDel);

    return retValue;
}

node* moveHeaps (node* heapD, node* heapS, struct idxInfo* idxArr, uint32_t heapDNum) {
    node* minHeap = NULL;

    uint8_t breakFlag = 0;
    for (;;) {
        if ((heapS = extractMin(heapS, &minHeap, 1)) == NULL)
            breakFlag = 1;

        minHeap->next   = NULL;
        minHeap->degree = 0;
        minHeap->lChild = NULL;
        idxArr[minHeap->idx].heapNum = heapDNum;

        heapD = unionHeap(mergeHeaps(heapD, minHeap));
        
        if (breakFlag)
            break;
    }

    return heapD;
}

int main () {
    uint32_t heapAmount = 0;
    uint32_t opAmount   = 0;
    scanf("%u %u", &heapAmount, &opAmount);

    node** heapsArr = calloc(heapAmount + 1, sizeof(node*));

    struct idxInfo* idxArr   = malloc(2 * sizeof(struct idxInfo));
    uint32_t curIdx  = 1;
    uint32_t arrSize = 2;

    uint32_t opNum = 0;
    for (uint32_t curOp = 0; curOp < opAmount; curOp++) {
        scanf("%u", &opNum);

        switch (opNum) {
        case 0: {
            uint32_t heapNum = 0;
            int32_t newValue = 0;

            scanf("%u %d", &heapNum, &newValue);

            idxArr[curIdx].elem    = NULL;
            idxArr[curIdx].heapNum = 0;
            idxArr[curIdx].isFree  = 0;

            heapsArr[heapNum] = binInsert(heapsArr[heapNum], newValue, &idxArr[curIdx].elem, curIdx);
            idxArr[curIdx++].heapNum = heapNum;

            if (curIdx == arrSize) {
                idxArr  = realloc(idxArr, (2 * arrSize + 1) * sizeof(struct idxInfo));
                arrSize = 2 * arrSize + 1;
            }

            break;
        }
        case 1: {
            uint32_t heapDNum = 0;
            uint32_t heapSNum = 0;

            scanf("%u %u", &heapSNum, &heapDNum);

            heapsArr[heapDNum] = moveHeaps(heapsArr[heapDNum], heapsArr[heapSNum], idxArr, heapDNum);
            heapsArr[heapSNum] = NULL;

            break;
        }
        case 2: {
            uint32_t elemIdx = 0;
            scanf("%u", &elemIdx);

            heapsArr[idxArr[elemIdx].heapNum] = deleteElem(heapsArr[idxArr[elemIdx].heapNum], idxArr[elemIdx].elem, idxArr);
            break;
        }
        case 3: {
            uint32_t elemIdx = 0;
            int32_t newValue = 0;

            scanf("%u %d", &elemIdx, &newValue);

            changeValue(idxArr[elemIdx].elem, newValue, idxArr);
            break;
        }
        case 4: {
            uint32_t heapNum = 0;
            scanf("%u", &heapNum);

            heapsArr[heapNum] = printMin(heapsArr[heapNum]);
            break;
        }
        case 5: {
            uint32_t heapNum = 0;
            scanf("%u", &heapNum);
            
            node* minNode = NULL;;
            heapsArr[heapNum] = extractMin(heapsArr[heapNum], &minNode, 1);

            idxArr[minNode->idx].isFree = 1;
            free(minNode);

            break;
        }
        default:
            curOp = UINT32_MAX - 1;
            break;
        }
    }

    for (uint32_t tempIdx = 1; tempIdx < curIdx; tempIdx++) {
        if (idxArr[tempIdx].isFree == 0)
            free(idxArr[tempIdx].elem);
    }

    free(heapsArr);
    free(idxArr);
}
