#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
        int n = 7;
        srand(time(NULL));
        int x, y;
        int hits = 0;
        for (size_t i = 0; i < 10000000; i++) {
                x = rand() % n;
                y = rand() % n;
                if (x == y) hits++;
        }
        printf("类型匹配成功了 %d 次\n", hits);
        return 0;
}
