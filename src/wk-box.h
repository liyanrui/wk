#ifndef WK_BOX_H
#define WK_BOX_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
        const char *type;
        size_t size;
        bool is_ref;
        void *body;
} WKBox;

WKBox *WK_BOX(void *data, size_t n, const char *type);
#define wk_box(val, T) WK_BOX(&(T){val}, sizeof(T), #T)
void wk_box_free(WKBox *box);

void WK_BOX_PUT(WKBox *box, void *data, size_t size, const char *type);
#define wk_box_put(box, val, T) WK_BOX_PUT(box, &(T){val}, sizeof(T), #T)

/* 引用：val 的地址会被复制到 box 里 */
#define wk_box_ref(val, T) &(WKBox){#T, sizeof(void *), true, (void *)&(T){val}}

#define wk_box_get(box, T) (*(T*)(box)->body)
#define wk_box_is(box, T) \
        (strcmp(box->type, T) == 0 ? true : false)

#endif
