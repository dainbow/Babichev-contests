#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int32_t myCmp(const void* elem1, const void* elem2) {
    char strConc1[200] = {};
    char strConc2[200] = {};

    strcat(strConc1, *(char**)elem1);
    strcat(strConc1, *(char**)elem2);

    strcat(strConc2, *(char**)elem2);
    strcat(strConc2, *(char**)elem1);

    return strcmp(strConc2, strConc1);
}

int main() {
    char** strings = calloc(100, sizeof(char*));

    for (uint32_t curString = 0; curString < 100; curString++) {
        strings[curString] = calloc(100, sizeof(char));
    }

    uint32_t stringCounter = 0;
    char tempChar = 0;
    while(scanf("%s\n", strings[stringCounter])) {
        stringCounter++;

        tempChar = getchar();
        if (tempChar == EOF) 
            break;
        else
            ungetc(tempChar, stdin);
    }

    qsort(strings, stringCounter, sizeof(char*), myCmp);

    for (uint32_t curString = 0; curString < stringCounter; curString++) {
        printf("%s", strings[curString]);
    }
    printf("\n");

    for (uint32_t curString = 0; curString < 100; curString++) {
        free(strings[curString]);
    }
    free(strings);
}