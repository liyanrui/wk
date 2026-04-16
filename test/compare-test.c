#include "wk-bus.h"

int main(void) {
	printf(">>> testing compare ...\n");
	WKBox *u, *v;
	u = wk_box_ref(3.14, double);
	v = wk_box_ref(2.7, double);
	WK_HOPE(wk_compare(u, v) == 1);
	v = wk_box_ref(3.14, double);
	WK_HOPE(wk_compare(u, v) == 0);
	printf("OK!\n");
	return 0;
}
