#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum lineTypes {
    row,
    column,
};

struct line {
    int32_t sum;
    
    enum lineTypes type;
    uint32_t number;
};

int32_t lineCmp(const void* elem1, const void* elem2) {
    return ((struct line*)elem1)->sum > ((struct line*)elem2)->sum;
}

int main() {
    uint32_t mHeight = 0;
    uint32_t mWidth  = 0;

    scanf("%u %u", &mHeight, &mWidth);

    struct line* matrixLines = calloc(sizeof(struct line), mHeight + mWidth);

    int32_t** matrix = calloc(sizeof(int32_t*), mWidth);
    for (uint32_t curColumn = 0; curColumn < mWidth; curColumn++) {
        matrix[curColumn] = calloc(sizeof(int32_t), mHeight);

        matrixLines[mHeight + curColumn].number = curColumn;
        matrixLines[mHeight + curColumn].type   = column;
    }

    int32_t tempValue = 0;
    for (uint32_t curRow = 0; curRow < mHeight; curRow++) {
        matrixLines[curRow].number = curRow;
        matrixLines[curRow].type   = row;
        
        for (uint32_t curColumn = 0; curColumn < mWidth; curColumn++) {
            scanf("%d", &tempValue);
            
            matrixLines[curRow].sum              += tempValue;
            matrixLines[mHeight + curColumn].sum += tempValue;

            matrix[curColumn][curRow] = tempValue;
        }
    }

    
    struct line* minElem = NULL;
    int32_t minSum       = INT32_MAX;

    for (uint32_t curLine = 0; curLine < mHeight + mWidth; curLine++) {
        // printf("%d ", matrixLines[curLine]);
        
        if (matrixLines[curLine].sum < minSum) {
            minElem = matrixLines + curLine;
            minSum  = matrixLines[curLine].sum;
        }
    }
    // printf("\n");

    while (minElem->sum < 0) {
        minElem->sum = -minElem->sum;

        if (minElem->type == row) {
            printf("l %u\n", minElem->number);

            for (uint32_t curColumn = 0; curColumn < mWidth; curColumn++) {
                matrix[curColumn][minElem->number] = -matrix[curColumn][minElem->number];
                matrixLines[mHeight + curColumn].sum += 2 * matrix[curColumn][minElem->number];
            }
        }
        else {
            printf("c %u\n", minElem->number);

            for (uint32_t curRow = 0; curRow < mHeight; curRow++) {
                matrix[minElem->number][curRow] = -matrix[minElem->number][curRow];
                matrixLines[curRow].sum += 2 * matrix[minElem->number][curRow];
            }
        }

        minSum       = INT32_MAX;
        for (uint32_t curLine = 0; curLine < mHeight + mWidth; curLine++) {
            // printf("%d ", matrixLines[curLine]);
            
            if (matrixLines[curLine].sum < minSum) {
                minElem = matrixLines + curLine;
                minSum  = matrixLines[curLine].sum;
            }
        }
        // printf("\n");
    }

    for (uint32_t curColumn = 0; curColumn < mWidth; curColumn++) {
        free(matrix[curColumn]);
    }

    free(matrix);
    free(matrixLines);
}