#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct binNode {
    struct binNode* parent;
    struct binNode* next;
    struct binNode* lChild;

    uint32_t idx;
    uint32_t degree;

    int32_t value;
} node;

struct idxInfo {
    struct binNode* elem;
    uint32_t heapNum;
};

node* mergeHeaps (node* heap1, node* heap2) {
    if (heap1 == NULL)
        return heap2;

    if (heap2 == NULL)
        return heap1;

    node* head = NULL;
    node* rememberHead = NULL;
    if ((heap1->degree < heap2->degree) || ((heap1->degree == heap2->degree) && (heap1->idx < heap2->idx))) {
        head = heap1;
        rememberHead = head;
        
        heap1 = heap1->next;
    }
    else {
        head = heap2;
        rememberHead = head;

        heap2 = heap2->next;
    }

    while (heap1 && heap2) {
        if ((heap1->degree < heap2->degree) || ((heap1->degree == heap2->degree) && (heap1->idx < heap2->idx))) {
            head->next = heap1;
            heap1 = heap1->next;
        }
        else {
            head->next = heap2;
            heap2 = heap2->next;
        }

        head = head->next;
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
    if (heap == NULL)
        return heap;

    if (heap->next == NULL)
        return heap;

    node* prevHeap = 0;
    node* curHeap  = heap;
    node* nextHeap = heap->next;

    while (nextHeap) {
        if ((curHeap->degree != nextHeap->degree) || (nextHeap->next && (nextHeap->next->degree == curHeap->degree))) {
            prevHeap = curHeap;
            curHeap =  nextHeap;
        }
        else {
            if ((curHeap->value < nextHeap->value) || ((curHeap->value == nextHeap->value) && (curHeap->idx < nextHeap->idx))) {
                curHeap->next    = nextHeap->next;
                nextHeap->parent = curHeap;
                nextHeap->next   = curHeap->lChild;
 
                curHeap->lChild = nextHeap;
                
                curHeap->degree++;
            }
            else {
                if (prevHeap) {
                    prevHeap->next = nextHeap;
                }
                else {
                    heap = nextHeap;
                }

                curHeap->parent = nextHeap;
                curHeap->next   = nextHeap->lChild;

                nextHeap->lChild = curHeap;
                nextHeap->degree++;

                curHeap  = nextHeap;
            }
        }

        nextHeap = curHeap->next;
    }

    return heap;
}

node* binInsert(node* head, int32_t value, node* newElemAdr, uint32_t idx) {
    newElemAdr->value  = value;
    newElemAdr->idx    = idx;

    return unionHeap(mergeHeaps(head, newElemAdr));
}

node* findMin(node* heap) {
    if (heap == NULL)
        return heap;

    node* prevNode     = NULL;

    node* minNode = heap;
    node* minPrev = NULL;

    while(heap) {
        if ((heap->value < minNode->value) || 
           ((heap->value == minNode->value) && (heap->idx < minNode->idx))) {
            minNode = heap;
            minPrev = prevNode;
        }

        prevNode = heap;
        heap = heap->next;
    }

    return minPrev;
}

node* extractNode(node* head, node* nodeToExtract) {
    node* minNode      = NULL;

    if (nodeToExtract == NULL) {
        minNode      = head;
        head = head->next;
    }
    else {
        minNode = nodeToExtract->next;
        nodeToExtract->next = nodeToExtract->next->next;
    }

    node* childHeap = NULL;
    node* child = minNode->lChild;

    while (child) {
        node* rememberNext = child->next;
        child->next   = childHeap;
        child->parent = NULL;

        childHeap = child;
        child = rememberNext;
    }

    return unionHeap(mergeHeaps(head, childHeap));
}

node* changeValue (node* elemToChange, int32_t value, struct idxInfo* idxArr) {
    if (value < elemToChange->value) {
        elemToChange->value = value;    
        while (elemToChange->parent && ((elemToChange->parent->value > elemToChange->value) || 
                                                                    ((elemToChange->parent->value == elemToChange->value) && 
                                                                     (elemToChange->parent->idx > elemToChange->idx)))) {
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
    else if (value > elemToChange->value) {
        elemToChange->value = value;

        for (;;) {
            node* child = elemToChange->lChild;
            
            node* minChild   = NULL;
            int32_t minValue = INT32_MAX;
            uint32_t minIdx  = UINT32_MAX;

            while (child) {
                if ((child->value < minValue) || ((child->value == minValue) && (child->idx < minIdx))) {
                    minChild = child;
                    minValue = child->value;
                    minIdx   = minChild->idx;
                }

                child = child->next;
            }          

            if ((minValue > elemToChange->value) || 
               ((minValue == elemToChange->value) && (minIdx > elemToChange->idx)))
                break;
            else {
                minChild->value     = elemToChange->value;
                elemToChange->value = minValue;

                idxArr[minChild->idx].elem = elemToChange;
                idxArr[elemToChange->idx].elem = minChild;

                minChild->idx       = elemToChange->idx;
                elemToChange->idx   = minIdx;

                elemToChange = minChild;
            }
        }
    }

    return elemToChange;
}

void printMin (node* heap) {
    node* minNode  = findMin(heap);
    if (minNode == NULL)
        minNode = heap;
    else 
        minNode = minNode->next;

    printf("%d\n", minNode->value);
}

node* deleteElem (node* heap, node* elemToDel, struct idxInfo* idxArr, node** heapsArr) {
    uint32_t rememberIdx = elemToDel->idx;
    idxArr[0] = idxArr[rememberIdx];
    elemToDel->idx = 0;

    changeValue(elemToDel, INT32_MIN, idxArr);
    idxArr[rememberIdx] = idxArr[0];

    return extractNode(heap, findMin(heap));
}

void ChangeHeapNum(node* heap, uint32_t newNum, struct idxInfo* idxArr) {
    idxArr[heap->idx].heapNum = newNum;

    if (heap->next)
        ChangeHeapNum(heap->next, newNum, idxArr);

    if (heap->lChild)
        ChangeHeapNum(heap->lChild, newNum, idxArr);
}

node* moveHeaps (node* heapD, node* heapS, struct idxInfo* idxArr, uint32_t heapDNum, uint32_t heapSNum, uint32_t maxIdx) {
    if (heapS == NULL)
        return heapD;
    
    ChangeHeapNum(heapS, heapDNum, idxArr);
        
    return unionHeap(mergeHeaps(heapD, heapS));
}

int main () {
    uint32_t heapAmount = 0;
    uint32_t opAmount   = 0;
    scanf("%u %u", &heapAmount, &opAmount);

    node** heapsArr = calloc(heapAmount + 1, sizeof(node*));

    struct idxInfo* idxArr   = malloc(1000000 * sizeof(struct idxInfo));
    uint32_t curIdx  = 1;
    
    for (uint32_t curOp = 0; curOp < opAmount; curOp++) {
        uint32_t opNum = 0;
        
        scanf("%u", &opNum);

        switch (opNum) {
        case 0: {
            uint32_t heapNum = 0;
            int32_t newValue = 0;

            scanf("%u %d", &heapNum, &newValue);
            idxArr[curIdx].elem    = calloc(1, sizeof(node));
            idxArr[curIdx].heapNum = heapNum;

            heapsArr[heapNum] = binInsert(heapsArr[heapNum], newValue, idxArr[curIdx].elem, curIdx);
            curIdx++;

            break;
        }
        case 1: {
            uint32_t heapDNum = 0;
            uint32_t heapSNum = 0;

            scanf("%u %u", &heapSNum, &heapDNum);
                
            heapsArr[heapDNum] = moveHeaps(heapsArr[heapDNum], heapsArr[heapSNum], idxArr, heapDNum, heapSNum, curIdx); 
            heapsArr[heapSNum] = NULL;
            
            break;
        }
        case 2: {
            uint32_t elemIdx = 0;
            scanf("%u", &elemIdx);

            heapsArr[idxArr[elemIdx].heapNum] = deleteElem(heapsArr[idxArr[elemIdx].heapNum], idxArr[elemIdx].elem, idxArr, heapsArr);

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

            printMin(heapsArr[heapNum]);
            break;
        }
        case 5: {
            uint32_t heapNum = 0;
            scanf("%u", &heapNum);
            
            heapsArr[heapNum] = extractNode(heapsArr[heapNum], findMin(heapsArr[heapNum]));

            break;
        }
        default:
            curOp = UINT32_MAX - 1;
            break;
        }
    }

    for (uint32_t tempIdx = 1; tempIdx < curIdx; tempIdx++) {
        free(idxArr[tempIdx].elem);
    }

    free(heapsArr);
    free(idxArr);
}
