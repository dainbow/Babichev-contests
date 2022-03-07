#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int32_t stringComp(const void* elem1, const void* elem2) {
    return strcmp(((char**)elem1)[0], ((char**)elem2)[0]);
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    assert(inputFile);

    const uint32_t maxStrAmount = 16; 
    const uint32_t maxStrSize   = 10003;

    char* stringBuffer[16] = {};

    for (uint32_t curString = 0; curString < maxStrAmount; curString++) {
        stringBuffer[curString] = calloc(sizeof(char), maxStrSize);
    }

    FILE* outputFile = fopen("temp.txt", "w");
    assert(outputFile);

    uint8_t whileFlag          = 0;
    uint32_t allStringsCounter = 0;
    uint32_t blockCounter      = 0;

    while (1) {
        uint32_t stringCounter = 0;

        for (uint32_t curString = 0; curString < maxStrAmount; curString++) {
            if (fgets(stringBuffer[curString], maxStrSize, inputFile) == NULL) {
                whileFlag = 1;
                break;
            }

            stringCounter++;
            allStringsCounter++;
        }
        
        qsort(stringBuffer, stringCounter, sizeof(uint8_t*), stringComp);

        for (uint32_t curString = 0; curString < stringCounter; curString++) {
            fputs(stringBuffer[curString], outputFile);
        }

        blockCounter++;
        if (whileFlag)
            break;
    }

    fclose(outputFile);
    fclose(inputFile);
    // printf("%u\n", allStringsCounter);
    
    uint32_t seriesAmount = maxStrAmount;

    while (seriesAmount < allStringsCounter) {
        outputFile = fopen("temp.txt", "r");
        assert(outputFile);

        FILE* tempOne = fopen("tmp1.txt", "w");
        assert(tempOne);

        FILE* tempSec = fopen("tmp2.txt", "w");
        assert(tempSec);

        uint8_t tempFlag      = 0;

        for (uint32_t curBlock = 0; curBlock < blockCounter; curBlock++) {
            for (uint32_t curString = 0; curString < seriesAmount; curString++) {
                if (fgets(stringBuffer[0], maxStrSize, outputFile) == NULL) {
                    break;
                }

                if (tempFlag) {
                    fputs(stringBuffer[0], tempOne);
                }
                else {
                    fputs(stringBuffer[0], tempSec);
                }
            }

            tempFlag ^= 1;
        }
        fclose(outputFile);
        remove("temp.txt");

        fclose(tempOne);
        fclose(tempSec);

        outputFile = fopen("temp.txt", "w");
        assert(outputFile);

        tempOne = fopen("tmp1.txt", "r");
        assert(tempOne);

        tempSec = fopen("tmp2.txt", "r");
        assert(tempSec);

        uint8_t flagT1End = 1;
        uint8_t flagT2End = 1;

        flagT1End = (fgets(stringBuffer[0], maxStrSize, tempOne) != NULL);
        flagT2End = (fgets(stringBuffer[1], maxStrSize, tempSec) != NULL);

        while(flagT1End || flagT2End) {
            uint32_t seriesAmount1 = 0;
            uint32_t seriesAmount2 = 0; 

            while(flagT1End && flagT2End && (seriesAmount1 < seriesAmount) && (seriesAmount2 < seriesAmount)) {
                if (memcmp(stringBuffer[0], stringBuffer[1], maxStrSize) < 0) {
                    fputs(stringBuffer[0], outputFile);

                    flagT1End = (fgets(stringBuffer[0], maxStrSize, tempOne) != NULL);
                    seriesAmount1++;
                }
                else {
                    fputs(stringBuffer[1], outputFile);

                    flagT2End = (fgets(stringBuffer[1], maxStrSize, tempSec) != NULL);
                    seriesAmount2++;
                }
            }

            while (flagT1End && (seriesAmount1 < seriesAmount)) {
                fputs(stringBuffer[0], outputFile);

                flagT1End = (fgets(stringBuffer[0], maxStrSize, tempOne) != NULL);
                seriesAmount1++;
            }

            while (flagT2End && (seriesAmount2 < seriesAmount)) {
                fputs(stringBuffer[1], outputFile);

                flagT2End = (fgets(stringBuffer[1], maxStrSize, tempSec) != NULL);
                seriesAmount2++;
            }
        }

        seriesAmount *= 2;
        blockCounter = (blockCounter >> 1) + (blockCounter & 1);
    
        fclose(outputFile);
        fclose(tempOne);
        fclose(tempSec);

        remove("tmp1.txt");
        remove("tmp2.txt");
    }

    for (uint32_t curString = 0; curString < maxStrAmount; curString++) {
        free(stringBuffer[curString]);
    }

    rename("temp.txt", "output.txt");
}
