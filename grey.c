#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void greyPrint(const uint64_t curLength, const uint64_t curNum) {
    if (curLength == 1) {
        printf("%lu\n", curNum - 1);
    }
    else {
        if (curNum > (1 << (curLength - 1))) {
            printf("1");
            greyPrint(curLength - 1, (1 << curLength) + 1 - curNum);
        }
        else {
            printf("0");
            greyPrint(curLength - 1, curNum);
        }
    }
}

int main() {
    uint64_t amount = 0;
    scanf("%lu", &amount);

    for (uint64_t curGNum = 1; curGNum <= (1 << amount); curGNum++) {
        greyPrint(amount, curGNum);
    }
}
