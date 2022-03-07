#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void makeSubstring(uint64_t const* lettersAmounts, const int8_t curLetter, const uint64_t curLetterAmount, int8_t* bufferString, const uint64_t curIdx) {
    if (curLetterAmount < lettersAmounts[curLetter]) {
        bufferString[curIdx]     = '0' + curLetter;
        bufferString[curIdx + 1] = '\0';

        printf("%s\n", bufferString);
        makeSubstring(lettersAmounts, curLetter, curLetterAmount + 1, bufferString, curIdx + 1);
    }

    if (curLetter < 74) {
        makeSubstring(lettersAmounts, curLetter + 1, 0, bufferString, curIdx);
    }
}

int main() {
    int8_t curChar      = 0;
    uint64_t fullAmount = 0;
    uint64_t lettersAmounts[75] = {};

    while ((curChar = getchar()) != '\n') {
        lettersAmounts[curChar - '0']++;
        fullAmount++;
    }

    int8_t* bufferString = calloc(sizeof(int8_t), fullAmount + 1);
    makeSubstring(lettersAmounts, 0, 0, bufferString, 0);
    printf("\n");

    free(bufferString);
}
