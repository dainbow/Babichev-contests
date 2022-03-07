#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    int64_t amount = 0;
    scanf("%ld", &amount);

    int64_t tempInt = 0;
    int64_t curSum  = 0;
    int64_t maxSum  = INT64_MIN;

    while (amount) {
        scanf("%ld", &tempInt);
    
        if (curSum + tempInt >= 0) {
            curSum = curSum + tempInt;
            maxSum = (curSum > maxSum) ? (curSum) : (maxSum);
        }
        else {
            curSum = 0;
            maxSum = (tempInt > maxSum) ? (tempInt) : (maxSum);
        }

        amount--;
    }

    printf("%ld\n", maxSum);
}
