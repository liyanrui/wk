#include <string.h>
#include "wk-err.h"
#include "wk-array.h"

WKArray *WK_ARRAY(size_t u) {
        WKArray *array = malloc(sizeof(WKArray));
        if (!array) wk_err("failed to create WKArray object");
        *array = (WKArray){0, u, 0, NULL};
        return array;
        wk_fallback_with(NULL);
}

void wk_array_free(WKArray *array) {
        free(array->body);
        free(array);
}

void WK_ARRAY_PUT(WKArray *array, size_t i, void *unit, size_t u) {
        if (array->u != u) wk_err("unit size not matched");
        if (i >= array->n) wk_err("out-of-bounds access");
        memcpy((char *)(array->body) + i * u, unit, u);
        wk_fallback;
}

void WK_ARRAY_ADD(WKArray *array, void *unit, size_t u) {
        if (array->u != u) wk_err("unit size not matched");
        if (!array->body) {
                array->body = malloc(u);
                if (!array->body) wk_err("memory not enough");
                array->m = u;
        } else {
                size_t t = (array->n + 1) * u;
                if (t > array->m) {
                        size_t m = t << 1; /* 容量倍增 */
                        void *new_body = realloc(array->body, m);
                        if (!new_body) wk_err("memory not enough");
                        array->body = new_body;
                        array->m = m;
                }
        }
        memcpy((char *)(array->body) + array->n * u, unit, u);
        array->n++;
        wk_fallback;
}

const void *WK_ARRAY_GET(WKArray *array, size_t i) {
        void *data = (char *)array->body + i * array->u;
        return data;
}

void wk_array_del(WKArray *array, size_t i) {
        if (i >= array->n) wk_err("out-of-bounds access");
        /* 将 [i + 1, array->n) 向左移一个单元 */
        char *a = array->body; /* 将 void * 转化为 char *，使之支持指针运算 */
        size_t u = array->u;
        size_t n = (array->n - 1 - i);
        if (n > 0) memmove(a + i * u, a + (i + 1) * u, n * u);
        array->n -= 1; /* 更新元素数量 */
        /* 缩容 */
        size_t new_m = (array->n << 1) * u;
        if (new_m > 0 && array->m > new_m) {
                void *new_body = realloc(array->body, new_m);
                if (!new_body) wk_err("memory not enough");
                array->body = new_body;
                array->m = new_m;
        }
        wk_fallback;
}
