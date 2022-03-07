#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    int32_t amount = 0;
    scanf("%d", &amount);

    int32_t* coinsArr = calloc(amount, sizeof(int32_t));

    int32_t amountNeed = 0;
    int32_t maxNeed    = 0;

    for (int32_t curCoin = 0; curCoin < amount; curCoin++) {
        scanf("%d", coinsArr + curCoin);

        if (coinsArr[curCoin] == 5)
            amountNeed--;
        else {
            amountNeed += (coinsArr[curCoin] - 5) / 5;

            if (amountNeed > maxNeed)
                maxNeed = amountNeed;
        }
    }

    if (maxNeed < 0)
        maxNeed = 0;

    printf("%d", maxNeed);

    free(coinsArr);
}
