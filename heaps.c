#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    int amount = 0;
    scanf("%d", &amount);

    int* array = (int*)calloc(amount, sizeof(int));
    int maxSum = 0;
    
    for (int curRock = 0; curRock < amount; curRock++) {
        scanf("%d", array + curRock);
        maxSum += array[curRock];
    }
    
    int minim = 99999999;
    for (int curState = 0; curState < (1 << amount); curState++) {
        int curStateCopy = curState;
        int sum = 0;
        int curBit  = amount - 1; 

        while (curStateCopy) { 
            sum -= (curStateCopy & 1) * array[curBit];

            curStateCopy = curStateCopy >> 1;
            curBit--;
        }     

        sum += maxSum + sum;
        sum = abs(sum);

        minim = (sum < minim) ? sum : minim;
        //minim = sum + ((minim - sum) & ((minim - sum) >> 31));
    }

    printf("%d\n", minim);
    free(array);
}
