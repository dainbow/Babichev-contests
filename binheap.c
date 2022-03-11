#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct binNode {
    int64_t value;

    struct binNode* parent;
    struct binNode* next;
    struct binNode* lChild;

    uint64_t degree;
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

node* binInsert(node* head, int64_t value) {
    node* newElem = calloc(1, sizeof(node));

    newElem->value  = value;

    return unionHeap(mergeHeaps(head, newElem));
}

node* extractMin(node* heap, int64_t* minValueAdr) {
    node* rememberHead = heap;
    node* prevNode     = NULL;

    int64_t minValue = heap->value;
    node* minNode = heap;
    node* minPrev = NULL;


    while(heap->next) {
        if (heap->value < minValue) {
            minNode = heap;
            minPrev = prevNode;

            minValue = heap->value;
        }

        prevNode = heap;
        heap = heap->next;
    }

    if (minPrev) {
        minPrev = minNode->next;
    }

    if (minValue)
        *minValueAdr = minNode->value;

    node* child = minNode->lChild;

    if (minNode == rememberHead)
        rememberHead = minNode->next;
    free(minNode);


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

void changeValue (node* elemToChange, int64_t value) {
    if (value < elemToChange->value) {
        elemToChange->value = value;
        while (elemToChange->parent && (elemToChange->parent->value > elemToChange->value)) {
            int64_t tempValue           = elemToChange->value;
            elemToChange->value         = elemToChange->parent->value;
            elemToChange->parent->value = tempValue;

            elemToChange = elemToChange->parent;
        }   
    }

    if (value > elemToChange->value) {
        elemToChange->value = value;

        for (;;) {
            node* child = elemToChange->lChild;
            
            node* minChild = NULL;
            int64_t minValue = INT64_MAX;

            while (child) {
                if (child->value < minValue) {
                    minValue = child->value;
                    minChild = child;
                }
            }          

            if (minValue > elemToChange->value)
                break;
            else {
                minChild->value     = elemToChange->value;
                elemToChange->value = minValue;

                elemToChange = minChild;
            }
        }
    }
}

node* printMin (node* heap) {
    int64_t minValue = 0;
    node* returnV = extractMin(heap, &minValue);

    printf("%ld\n", minValue);
    return returnV;
}

node* deleteElem (node* heap, node* elemToDel) {
    changeValue(elemToDel, INT64_MIN);
    return extractMin(heap, NULL);
}

int main () {
    uint32_t heapAmount = 0;
    uint32_t opAmount   = 0;
    scanf("%u %u", &heapAmount, &opAmount);

    node** heapsArr = calloc(heapAmount, sizeof(node*));
    struct idxInfo* idxArr   = calloc(1000000, sizeof(struct idxInfo));

    uint32_t opNum = 0;
    for (uint32_t curOp = 0; curOp < opAmount; curOp++) {
        scanf("%u", opNum);

        switch (opNum) {
        case 2: {
            uint32_t elemIdx = 0;
            scanf("%u");

            deleteElem(idxArr[elemIdx].heapNum, idxArr[elemIdx].elem);
            break;
        }
        case 3: {
            uint32_t elemIdx = 0;
            int64_t newValue = 0;

            scanf("%u %ld", &elemIdx, &newValue);

            changeValue(idxArr[elemIdx], newValue);
            break;
        }
        case 4: {
            uint32_t heapNum = 0;
            scanf("%u", &heapNum);

            printMin(heapsArr[heapNum]);
        }
            
        
        default:
            break;
        }
    }

    printf("OK\n");
}

// if (heap->value < heap->next->value) {
//                         heap->next = heap->next->next;
//                         heap->next->next = heap->lChild;
//                         heap->next->parent     = heap;

//                         heap->lChild = heap->next;
//                     }