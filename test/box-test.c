#include "wk-err.h"
#include "wk-box.h"

int main(void) {
        printf(">>> testing box ...\n");
        WKBox *a = wk_box(42, int);
        WKBox *b = wk_box_ref(42, int);
        WK_HOPE(wk_box_get(a, int) == wk_box_get(b, int));
        wk_box_free(a); /* 释放 a */
        
        int x = 42; int *p = &x;
        WKBox *c = wk_box(p, int *);
        WK_HOPE(wk_box_get(c, int *) == &x);
        wk_box_free(c);

        printf("OK!\n");
        return 0;
}
