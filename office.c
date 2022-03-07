#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct time {
    uint64_t timeValue;
    int8_t inOutV;
};

int timeCmp(const void* elem1, const void* elem2) {
    if (((struct time*)elem1)->timeValue > ((struct time*)elem2)->timeValue)
        return 1;
    
    if (((struct time*)elem1)->timeValue == ((struct time*)elem2)->timeValue)
        return ((struct time*)elem1)->inOutV < ((struct time*)elem2)->inOutV;
    
    return -1;
}

int main() {
    uint32_t amount = 0;
    scanf("%u", &amount);

    struct time* timeArr = calloc(sizeof(struct time), amount * 2);
    
    uint32_t hourTemp = 0;
    uint32_t minTemp  = 0;
    uint32_t secTemp  = 0;

    for (uint32_t curPerson = 0; curPerson < 2 * amount; curPerson += 2) {
        scanf("%u:%u:%u", &hourTemp, &minTemp, &secTemp);

        timeArr[curPerson].timeValue = 3600 * hourTemp + 60 * minTemp + secTemp;
        timeArr[curPerson].inOutV    = 1;

        scanf("%u:%u:%u", &hourTemp, &minTemp, &secTemp);

        timeArr[curPerson + 1].timeValue = 3600 * hourTemp + 60 * minTemp + secTemp;
        timeArr[curPerson + 1].inOutV    = -1;
    }

    qsort(timeArr, amount * 2, sizeof(struct time), timeCmp);

    int32_t curAmount = 0;
    int32_t maxAmount = 0;

    for (uint32_t curPerson = 0; curPerson < 2 * amount; curPerson++) {
        curAmount += timeArr[curPerson].inOutV;

        maxAmount = (curAmount > maxAmount) ? curAmount : maxAmount;
    }

    printf("%u\n", maxAmount);

    free(timeArr);
}
