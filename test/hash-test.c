#include "wk-str.h"

int main(void) {
        printf(">>> testing hash ...\n");
        const char *raw = "hello";
        WKStr *a = wk_str(raw);
        WKBox *x = wk_box_ref(raw, const char *);
        WKBox *y = wk_box(a, WKStr *);
        size_t ha = wk_hash(x);
        size_t hb = wk_hash(y);
        WK_HOPE(ha == hb);
        wk_str_free(a);
        wk_box_free(x);
        wk_box_free(y);
        printf("OK!\n");
        return 0;
}
