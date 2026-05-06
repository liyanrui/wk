#ifndef WK_BOX_H
#define WK_BOX_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
        const char *type;
        bool is_ref;
        void *body;
} WKBox;

WKBox *WK_BOX(void *data, size_t n, const char *type);
#define wk_box(val, T) WK_BOX(&(T){val}, sizeof(T), #T)
void wk_box_free(WKBox *box);

/* 引用：val 的地址会被复制到 box 里 */
#define wk_box_ref(val, T) &(WKBox){#T, true, (void *)&(T){val}}

#define wk_box_get(box, T) (*(T*)(box)->body)
#define wk_box_is(box, T) \
        (strcmp(box->type, T) == 0 ? true : false)

#endif
