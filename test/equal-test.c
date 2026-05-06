#include "wk-v.h"

int main(void) {
	wk_v_begin;
	printf(">>> testing equal ...\n");
	wk_bus_init();
	WKBox *u, *v;
        /* 引用，无需释放 */
        u = wk_box_ref(42, int);
        v = wk_box_ref(42, int);
        WK_HOPE(wk_equal(u, v));

        /* 指针比较 */
	double pi = 3.14;
	void *a = &pi;
	void *b = &pi;
	u = wk_box_ref(a, void *);
	v = wk_box_ref(b, void *);
	WK_HOPE(wk_equal(u, v));
	
        /* C 字符串与 WKStr 对象比较 */
        u = wk_box_ref("hi", const char *);
        v = wk_v_box(wk_str("hi"), WKStr *);
        WK_HOPE(wk_equal(u, v));
	
        printf("OK!\n");
        wk_v_ret(0);
}
