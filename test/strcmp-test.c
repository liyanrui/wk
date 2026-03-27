#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
        char *types[] = {"int",
                         "double",
                         "long long ago",
                         "红烧狮子头",
                         "淮海省是个危险的地方……",
                         "红烧茄子也不错！",
                         "WKObj *"};
        int n = 7;
        srand(time(NULL));
        int x, y;
        int hits = 0;
        for (size_t i = 0; i < 10000000; i++) {
                x = rand() % n;
                y = rand() % n;
                if (strcmp(types[x], types[y]) == 0) hits++;
        }
        printf("类型匹配成功了 %d 次\n", hits);
        return 0;
}
