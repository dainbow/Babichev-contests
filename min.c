#include <stdio.h>

int mini(int x, int y) {
    return y + ((x - y) & ((x - y) >> 31));
}

int main() {
    int a = 0;
    int b = 0;

    scanf("%d %d", &a, &b);

    printf("Min is %d\n", mini(a, b));
}