#ifndef WK_ENTRY_H
#define WK_ENTRY_H
#include <wk-bus.h>

typedef struct {
        WKBox *x;
        WKBox *y;
} WKPair;

WKPair *wk_pair(WKBox *x, WKBox *y);

/* 不负责 x 和 y 的释放 */
#define wk_pair_free free

#endif
