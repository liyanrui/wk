#include <math.h> /* for fabs */
#include "wk-bus.h"

/* free 总线 */
wk_bus_define(WKFree, wk_free);
/* 基本类型的盒子，slot->action 是 NULL */
void wk_free(WKBox *a) {
	if (!_wk_free_bus_) wk_err("unknown type!");
        for (size_t i = 0; i < _wk_free_bus_->n; i++) {
                WKFreeSlot *slot = wk_array_get(_wk_free_bus_, i, WKFreeSlot *);
                if (wk_box_is(a, slot->type)) {
			if (slot->action) slot->action(a);
                        return;
                }
        }
	wk_fallback;
}

/* hash 总线函数 */
wk_bus_define(WKHash, wk_hash);
/* wk_hash 无渗透性，即 slot->action 作用于盒子本身 */
size_t wk_hash(WKBox *a) {
	static bool first_run = true;
	if (first_run) {
		first_run = false;
		wk_equal_bus_connect("int", NULL);
		wk_equal_bus_connect("size_t", NULL);
		wk_equal_bus_connect("void *", NULL);
		wk_equal_bus_connect("float", NULL);
		wk_equal_bus_connect("double", NULL);
	}
        for (size_t i = 0; i < _wk_hash_bus_->n; i++) {
                WKHashSlot *slot = wk_array_get(_wk_hash_bus_, i, WKHashSlot *);
                if (wk_box_is(a, slot->type)) {
                        return slot->action(a);
                }
        }
	wk_err("unknown type!");
	wk_fallback_with(0);
}

/* equal 总线函数 */
/* 用户无法提供可以渗透盒子内部的函数，故而只能由用户提供在盒子层面的比较函数 */
wk_bus_define(WKEqual, wk_equal);
bool wk_equal(WKBox *a, WKBox *b) {
	static bool first_run = true;
	if (first_run) {
		first_run = false;
		wk_equal_bus_connect("int", NULL);
		wk_equal_bus_connect("size_t", NULL);
		wk_equal_bus_connect("void *", NULL);
		wk_equal_bus_connect("float", NULL);
		wk_equal_bus_connect("double", NULL);
	}
        for (size_t i = 0; i < _wk_equal_bus_->n; i++) {
                WKEqualSlot *slot = wk_array_get(_wk_equal_bus_, i, WKEqualSlot *);
		if (wk_box_is(a, "int") || wk_box_is(b, "int")) {
			return wk_box_get(a, int) == wk_box_get(b, int) ? true : false;
		}
		if (wk_box_is(a, "size_t") || wk_box_is(b, "size_t")) {
			return wk_box_get(a, size_t) == wk_box_get(b, size_t) ? true : false;
		}
		if (wk_box_is(a, "void *") || wk_box_is(b, "void *")) {
			return wk_box_get(a, void *) == wk_box_get(b, void *) ? true : false;
		}
		if (wk_box_is(a, "float") || wk_box_is(b, "float")) {
			float x = wk_box_get(a, float);
			float y = wk_box_get(b, float);
			return fabsf(x - y) < wk_flt_eps ? true : false;
		}
		if (wk_box_is(a, "doble") || wk_box_is(b, "double")) {
			double x = wk_box_get(a, double);
			double y = wk_box_get(b, double);
			return fabs(x - y) < wk_dbl_eps ? true : false;
		}		
                if (wk_box_is(a, slot->type) || wk_box_is(b, slot->type)) {
                        return slot->action(a, b);
                }
        }
	wk_err("unknown type!");
        wk_fallback_with(false);
}

/* compare 总线函数 */
/* 用户无法提供可以渗透盒子内部的函数，故而只能由用户提供在盒子层面的比较函数 */
wk_bus_define(WKCompare, wk_compare);
int wk_compare(WKBox *a, WKBox *b) {
	static bool first_run = true;
	if (first_run) {
		first_run = false;
		wk_compare_bus_connect("int", NULL);
		wk_compare_bus_connect("size_t", NULL);
		wk_compare_bus_connect("float", NULL);
		wk_compare_bus_connect("double", NULL);
	}
        for (size_t i = 0; i < _wk_compare_bus_->n; i++) {
                WKCompareSlot *slot = wk_array_get(_wk_compare_bus_, i, WKCompareSlot *);
		if (wk_box_is(a, "int") || wk_box_is(b, "int")) {
			int x = wk_box_get(a, int);
			int y = wk_box_get(b, int);
			if (x < y) return -1;
			if (x == y) return 0;
			if (x > y) return 1;
		}
		if (wk_box_is(a, "size_t") || wk_box_is(b, "size_t")) {
			size_t x = wk_box_get(a, size_t);
			size_t y = wk_box_get(b, size_t);
			if (x < y) return -1;
			if (x == y) return 0;
			if (x > y) return 1;
		}
		if (wk_box_is(a, "float") || wk_box_is(b, "float")) {
			float x = wk_box_get(a, float);
			float y = wk_box_get(b, float);
			if (fabsf(x - y) < wk_flt_eps) return 0;
			if (x < y) return -1;
			if (x > y) return 1;
		}
		if (wk_box_is(a, "double") || wk_box_is(b, "double")) {
			double x = wk_box_get(a, double);
			double y = wk_box_get(b, double);
			if (fabs(x - y) < wk_dbl_eps) return 0;
			if (x < y) return -1;
			if (x > y) return 1;
		}
		if (wk_box_is(a, slot->type) || wk_box_is(b, slot->type)) {
			return slot->action(a, b);
                }
        }
	wk_err("unknown type!");
        wk_fallback_with(-2);
}
