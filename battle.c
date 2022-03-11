#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main() {
    uint64_t dragonAmount = 0;
    int64_t mainDamage   = 0;
    int64_t sideDamage   = 0;

    scanf("%lu %ld %ld", &dragonAmount, &mainDamage, &sideDamage);
    int64_t* dragonHps = calloc(dragonAmount, sizeof(int64_t));
    
    int64_t tempValue = 0;
    int64_t maxDragon = 0;
    for (uint64_t curDragon = 0; curDragon < dragonAmount; curDragon++) {
        scanf("%ld", &tempValue);
        maxDragon = (tempValue > maxDragon) ? tempValue : maxDragon;

        dragonHps[curDragon] = tempValue; 
    }
    int64_t answer = INT64_MAX;



    if (sideDamage != mainDamage) {
        int64_t left   = 0;
        int64_t right  = (maxDragon / sideDamage) + ((maxDragon % sideDamage) != 0);
        int64_t middle = 0;

        while (left <= right) {
            middle = (left + right) / 2;
            int64_t curRemain = 0;

            for (uint64_t curDragonN = 0; curDragonN < dragonAmount; curDragonN++) {
                int64_t curDragonV = ((dragonHps[curDragonN] - sideDamage * middle) > 0) ? dragonHps[curDragonN] - sideDamage * middle : 0;

                curRemain += (curDragonV / (mainDamage - sideDamage)) + ((curDragonV % (mainDamage - sideDamage)) != 0);
            }

            if (curRemain > middle)
                left = middle + 1;
            else {
                answer = (middle < answer) ? middle : answer;
                right = middle - 1;
            }
        }
    }
    else {
        answer = (maxDragon / sideDamage) + ((maxDragon % sideDamage) != 0);
    }

    printf("%ld\n", answer);

    free(dragonHps);
}