#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    uint32_t amount = 0;
    uint32_t number = 0;

    scanf("%u %u", &amount, &number);
//20, 2, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
    uint32_t counter = 1;
    while(number != 1) {
        if ((number % 10) == 0) {
            number /= 10;
            counter ++;

            continue;
        }

        number--;
        counter++;

        for (int8_t curDigit = 9; curDigit >= 0; curDigit--) {
            while((10 * number + curDigit) <= amount) {
                number = 10 * number + curDigit;
            }
        }
    }

    printf("%u\n", counter);
}
