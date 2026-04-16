#include "wk-str.h"

int main(void) {
	wk_v_begin;
	printf(">>> testing equal ...\n");
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
        v = wk_box_ref(wk_str_v("hi"), WKStr *);
        WK_HOPE(wk_equal(u, v));
        u = wk_box_ref(wk_str_v("hello"), WKStr *);
	v = wk_box_ref(wk_str_v("hello"), WKStr *);
	WK_HOPE(wk_equal(u, v));
        v = wk_box_ref(wk_str_v("World"), WKStr *);
        WK_HOPE(!wk_equal(u, v));
	
        printf("OK!\n");
        wk_v_ret(0);
}
