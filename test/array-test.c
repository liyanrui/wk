#include "wk-err.h"
#include "wk-array.h"
int main(void) {
        printf(">>> testing array...\n");
        WKArray *x = wk_array(int);
        /* 将 a 中元素复制到 x */
        int a[] = {0, 1, 2, 3, 4};
        size_t n = sizeof(a) / sizeof(int);
        for (size_t i = 0; i < n; i++) {
                wk_array_add(x, a[i], int);
        }
        /* 添加字面量 */
        wk_array_add(x, 5, int);
        /* 删除 */
        wk_array_del(x, 0);
        wk_array_del(x, 2);
        wk_array_del(x, x->n - 1);
        WK_HOPE(x->n == 3);
        /* 遍历 x */
        int b[] = {1, 2, 4};
        bool success = true;
        for (size_t i = 0; i < x->n; i++) {
                int t = wk_array_get(x, i, int);
                if (t != b[i]) {
                        success = false;
                        break;
                }
        }
        WK_HOPE(success);
        wk_array_free(x);
        printf("OK!\n");
        return 0;
}
