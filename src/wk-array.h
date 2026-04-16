#ifndef WK_ARRAY_H
#define WK_ARRAY_H
#include <stdio.h>

typedef struct wk_array {
        size_t n; /* 数组存储 n 个元素 */
        size_t u; /* 每个元素占用的内存为 u 字节 */
        size_t m; /* 数组空间所占内存为 m 字节 */
        void *body;
} WKArray;

WKArray *WK_ARRAY(size_t u);
#define wk_array(type) WK_ARRAY(sizeof(type))
void wk_array_free(WKArray *array);

void WK_ARRAY_ADD(WKArray *array, void *unit, size_t u);
#define wk_array_add(array, U, T) do { \
        T _tmp_unit_ = U; \
        WK_ARRAY_ADD(array, &_tmp_unit_, sizeof(T)); \
} while (0)

void WK_ARRAY_PUT(WKArray *array, size_t i, void *unit, size_t u);
#define wk_array_put(array, index, U, T) do {     \
        T _tmp_unit_ = U; \
        WK_ARRAY_PUT(array, index, &_tmp_unit_, sizeof(T)); \
} while (0)

const void *WK_ARRAY_GET(WKArray *array, size_t i);
#define wk_array_get(array, i, T) (*(T*)WK_ARRAY_GET(array, i))

void wk_array_del(WKArray *array, size_t i);

#endif
