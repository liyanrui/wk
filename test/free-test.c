#include "wk-str.h"
#include "wk-free.h"

int main(void) {
        printf(">>> testing free...\n");
        size_t n = 4;
        WKBox *boxes[n];
        boxes[0] = wk_box(42, int);
        boxes[1] = wk_box(3.14, double);
        boxes[2] = wk_box("Hello world!", const char *);
        boxes[3] = wk_box(wk_str("Hello world again!"), WKStr *);
        boxes[4] = wk_box_ref(n, size_t);
        for (size_t i = 0; i < n; i++) {
                wk_free(boxes[i]);
        }
        printf("OK!\n");
        return 0;
}
