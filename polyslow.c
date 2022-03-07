#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t power(const uint64_t base, uint64_t pow, const uint64_t module);
uint64_t calculate(const uint64_t curX, const uint64_t* coefArg, const uint64_t module, const uint64_t degree);

int main() {
    uint64_t degree   = 0;
    uint64_t amountOf = 0;
    uint64_t module   = 0;

    scanf("%d %d %d", &degree, &amountOf, &module);

    uint64_t* coefArr = calloc(degree + 1, sizeof(uint64_t));
    uint64_t* xArr    = calloc(amountOf, sizeof(uint64_t));

    for (uint64_t curCoef = 0; curCoef <= degree; curCoef++) {
        scanf("%d", coefArr + curCoef);
    }

    for (uint64_t curX = 0; curX < amountOf; curX++) {
        scanf("%d", xArr + curX);
    }

    for (uint64_t curX = 0; curX < amountOf; curX++) {
        printf("%u\n", calculate(xArr[curX], coefArr, module, degree));
    }

}

uint64_t calculate(const uint64_t curX, const uint64_t* coefArg, const uint64_t module, const uint64_t degree) {
    uint64_t result = 0;

    for (uint64_t curCoef = 0; curCoef <= degree; curCoef++) {
        result += (coefArg[curCoef] * power(curX, degree - curCoef, module)) % module;
    }

    return (result % module);
}

uint64_t power(const uint64_t base, uint64_t pow, const uint64_t module) {
    uint64_t result = 1;

    for (; pow; pow--) {
        result = (result * base) % module;
    }

    return result;
}
