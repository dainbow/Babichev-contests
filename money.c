#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    uint64_t amount = 0;
    scanf("%lu", &amount);

    uint64_t* coinsArr = malloc(amount * sizeof(uint64_t));

    uint64_t maxSum    = 0;
    uint64_t answer    = 0;
    for (uint64_t curCoin = 0; curCoin < amount; curCoin++) {
        scanf("%lu", coinsArr + curCoin);

        if ((answer == 0) && (maxSum < coinsArr[curCoin] - 1)) {
            answer = maxSum + 1;
        }

        maxSum += coinsArr[curCoin];
    }

    if (answer == 0) {
        answer = maxSum + 1;
    }

    printf("%lu\n", answer);

    free(coinsArr);
}
