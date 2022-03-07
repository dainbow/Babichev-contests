#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int main() {
    const uint32_t maxStrSize = 10003;
    
    FILE* outputF = fopen("output.txt", "rb");

    char* str1 = calloc(sizeof(char), maxStrSize);
    char* str2 = calloc(sizeof(char), maxStrSize);

    uint32_t strCount = 0;

    fgets(str1, maxStrSize, outputF);
    strCount++;

    while(fgets(str2, maxStrSize, outputF) != NULL) {
        assert(memcmp(str1, str2, maxStrSize) <= 0);

        strCount++;
        strncpy(str1, str2, maxStrSize);
    }
    printf("%d\n", strCount);



    free(str1);
    free(str2);
    fclose(outputF);
}

