#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void printNSym(const uint32_t symN, const uint32_t stringN) {
    if (symN == 1) {
        printf("%c", 'a' - 1 + stringN);
        return;
    }
    
    if ((stringN == 2) || (symN == UINT32_MAX)) {
        printf("a");
        return;
    }

    printNSym((symN % ((1 << (stringN - 1)) - 1)) - 1, stringN - 1);
    return;
}

int main() {
    uint32_t strNum   = 0;
    uint32_t leftNum  = 0;
    uint32_t rightNum = 0;

    scanf("%u %u %u", &strNum, &leftNum, &rightNum);

    for (uint32_t curSym = leftNum; curSym <= rightNum; curSym++) {
        printNSym(curSym, strNum);
    }
    printf("\n");
}

// edcbaabaacbaabaadcbaabaacbaabaa
