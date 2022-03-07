#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void buildTree(int32_t* treeArr, int32_t* intArr, int32_t curIdx, int32_t curLeft, int32_t curRight) {
    if (curLeft == curRight) {
        treeArr[curIdx] = intArr[curLeft];
    }
    else {
        int32_t curMiddle = (curLeft + curRight) / 2;

        buildTree(treeArr, intArr, 2 * curIdx, curLeft, curMiddle);
        buildTree(treeArr, intArr, 2 * curIdx + 1, curMiddle + 1, curRight);
    }
}

void subArrAdd(int32_t* treeArr, int32_t curIdx, int32_t curLeft, int32_t curRight, int32_t opLeft, int32_t opRight, int32_t addValue) {
    if (opLeft > opRight) {
        return;
    }
    else if ((opLeft == curLeft) && (opRight == curRight)) {
        treeArr[curIdx] += addValue;
    }
    else {
        int32_t curMiddle = (curLeft + curRight) / 2;

        if (opRight <= curMiddle) {
            subArrAdd(treeArr, 2 * curIdx, curLeft, curMiddle, opLeft, opRight, addValue);
        }
        else if (opLeft > curMiddle) {
            subArrAdd(treeArr, 2 * curIdx + 1, curMiddle + 1, curRight, opLeft, opRight, addValue);
        }
        else {
            subArrAdd(treeArr, 2 * curIdx, curLeft, curMiddle, opLeft, curMiddle, addValue);
            subArrAdd(treeArr, 2 * curIdx + 1, curMiddle + 1, curRight, curMiddle + 1, opRight, addValue);
        }
    }
}

int32_t getInt(int32_t* treeArr, int32_t curIdx, int32_t searchIdx, int32_t curLeft, int32_t curRight) {
    if (curLeft == curRight) {
        return treeArr[curIdx];
    }

    int32_t curMiddle = (curLeft + curRight) / 2;

    if (searchIdx <= curMiddle) {
        return treeArr[curIdx] + getInt(treeArr, 2 * curIdx, searchIdx, curLeft, curMiddle);
    }
    else {
        return treeArr[curIdx] + getInt(treeArr, 2 * curIdx + 1, searchIdx, curMiddle + 1, curRight);
    }
}

int main() {
    int32_t arrSize  = 0;
    int32_t opAmount = 0;

    scanf("%d %d", &arrSize, &opAmount);

    int32_t* intArr = calloc(sizeof(int32_t), arrSize);
    for (int32_t curElem = 0; curElem < arrSize; curElem++) {
        scanf("%d", intArr + curElem);
    }

    uint32_t treeSize = 1;
    while (treeSize < arrSize) {
        treeSize = treeSize << 1;
    }

    int32_t* treeArr = calloc(sizeof(int32_t), 2 * treeSize);
    buildTree(treeArr, intArr, 1, 0, arrSize - 1);
    free(intArr);

    int32_t opLeft   = 0;
    int32_t opRight  = 0;
    int32_t addValue = 0;

    for (int32_t curOp = 0; curOp < opAmount; curOp++) {
        scanf("%d %d %d", &opLeft, &opRight, &addValue);

        subArrAdd(treeArr, 1, 0, arrSize - 1, opLeft, opRight, addValue);
    }

    for (int32_t curElem = 0; curElem < arrSize; curElem++) {
        int32_t curValue = getInt(treeArr, 1, curElem, 0, arrSize - 1);
    
        printf("%d ", curValue);
    }
    printf("\n");

    free(treeArr);
}