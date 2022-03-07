#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

uint64_t calculate(const uint64_t curX, const uint64_t* coefArg, const uint64_t module, const uint64_t degree);

int main() {
    uint64_t degree   = 0;
    uint64_t amountOf = 0;
    uint64_t module   = 0;

    scanf("%lu %lu %lu", &degree, &amountOf, &module);

    uint64_t* coefArr = calloc(sizeof(uint64_t), degree + 1);
    uint64_t* xArr    = calloc(sizeof(uint64_t), amountOf);

    for (uint64_t curCoef = 0; curCoef <= degree; curCoef++) {
        scanf("%lu", coefArr + curCoef);
    }

    for (uint64_t curX = 0; curX < amountOf; curX++) {
        scanf("%lu", xArr + curX);
    }

    for (uint64_t curX = 0; curX < amountOf; curX++) {
        printf("%lu\n", calculate(xArr[curX], coefArr, module, degree));
    }

    free(coefArr);
    free(xArr);
}

uint64_t calculate(const uint64_t curX, const uint64_t* coefArg, const uint64_t module, const uint64_t degree) {
    assert(coefArg   != NULL);
    
    uint64_t result = 0;
    uint64_t curDegree = 1;

    for (uint64_t curCoef = 0; curCoef <= degree; curCoef++) {
        result += (coefArg[degree - curCoef] * curDegree) % module;

        curDegree = (curDegree * curX) % module;
    }

    return (result % module);
}

