#include<stdint.h> /* for uintptr_t */ 
#include <math.h>  /* for fabs */
#include "wk-bus.h"

/* free 总线 */
wk_bus_define(WKFree, wk_free);
static void wk_str_box_free(WKBox *a) {
	wk_str_free(wk_box_get(a, WKStr *));
	wk_box_free(a);
}
void wk_free_bus_init(void) {
	wk_free_bus_connect("int", wk_box_free);
	wk_free_bus_connect("size_t", wk_box_free);
	wk_free_bus_connect("float", wk_box_free);
	wk_free_bus_connect("double", wk_box_free);
	wk_free_bus_connect("void *", wk_box_free);
	wk_free_bus_connect("const char *", wk_box_free);
	wk_free_bus_connect("WKStr *", wk_str_box_free);
}
void wk_free(WKBox *a) {
	if (!_wk_free_bus_) wk_err("bus not created");
        for (size_t i = 0; i < _wk_free_bus_->n; i++) {
                WKFreeSlot *slot = wk_array_get(_wk_free_bus_, i, WKFreeSlot *);
                if (wk_box_is(a, slot->type)) {
			slot->action(a);
			return;
                }
        }
	wk_err("unknown type");
	wk_fallback;
}

/* hash 总线函数 */
wk_bus_define(WKHash, wk_hash);
static size_t wk_int_hash(WKBox *a) { return (size_t)wk_box_get(a, int); }
static size_t wk_size_t_hash(WKBox *a) { return wk_box_get(a, size_t); }
static size_t wk_float_hash(WKBox *a) { return (size_t)wk_box_get(a, float); }
static size_t wk_double_hash(WKBox *a) { return (size_t)wk_box_get(a, double); }
static size_t wk_pointer_hash(WKBox *a) { return (size_t)wk_box_get(a, void *); }
static const char *wk_str_box_get(WKBox *a) {
        const char *x;
        if (wk_box_is(a, "WKStr *")) {
                x = wk_box_get(a, WKStr *)->body;
        } else if (wk_box_is(a, "const char *") || wk_box_is(a, "char *")) {
                x = wk_box_get(a, const char *);
        } else wk_err("invalid string object!");
	return x;
	wk_fallback_with(NULL);
}
static size_t wk_raw_str_box_hash(WKBox *a) {
	const char *raw = wk_box_get(a, const char *);
        size_t hash = 5381;
        while (*raw != '\0') {
                hash = (hash << 5) + hash + *raw;
                raw++;
        }
        return hash;
}
static size_t wk_str_box_hash(WKBox *a) {
	WKStr *str = wk_box_get(a, WKStr *);
	WKBox *b = wk_box_ref(str->body, const char *);
        return wk_raw_str_box_hash(b);
}
void wk_hash_bus_init(void) {
	wk_hash_bus_connect("int", wk_int_hash);
	wk_hash_bus_connect("size_t", wk_size_t_hash);
	wk_hash_bus_connect("float", wk_float_hash);
	wk_hash_bus_connect("double", wk_double_hash);
	wk_hash_bus_connect("void *", wk_pointer_hash);
	wk_hash_bus_connect("const char *", wk_raw_str_box_hash);
	wk_hash_bus_connect("WKStr *", wk_str_box_hash);
}
size_t wk_hash(WKBox *a) {
	if (!_wk_hash_bus_) wk_err("bus not created!");
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
wk_bus_define(WKEqual, wk_equal);
static bool wk_int_equal(WKBox *a, WKBox *b) {
	return wk_box_get(a, int) == wk_box_get(b, int) ? true : false;
}
static bool wk_size_t_equal(WKBox *a, WKBox *b) {
	return wk_box_get(a, size_t) == wk_box_get(b, size_t) ? true : false;
}
static bool wk_float_equal(WKBox *a, WKBox *b) {
	return (fabs(wk_box_get(a, float) - wk_box_get(b, float)) < wk_flt_eps) ? true : false;
}
static bool wk_double_equal(WKBox *a, WKBox *b) {
	return (fabs(wk_box_get(a, double) - wk_box_get(b, double)) < wk_dbl_eps) ? true : false;
}
static bool wk_pointer_equal(WKBox *a, WKBox *b) {
	return wk_box_get(a, void *) == wk_box_get(b, void *) ? true : false;
}
static bool wk_str_box_eq(WKBox *a, WKBox *b) {
        const char *x = wk_str_box_get(a);
	const char *y = wk_str_box_get(b);
        return strcmp(x, y) == 0 ? true : false;
}
void wk_equal_bus_init(void) {
	wk_equal_bus_connect("int", wk_int_equal);
	wk_equal_bus_connect("size_t", wk_size_t_equal);
	wk_equal_bus_connect("float", wk_float_equal);
	wk_equal_bus_connect("double", wk_double_equal);
	wk_equal_bus_connect("void *", wk_pointer_equal);
	wk_equal_bus_connect("const char *", wk_str_box_eq);
	wk_equal_bus_connect("WKStr *", wk_str_box_eq);
}
bool wk_equal(WKBox *a, WKBox *b) {
	if (!_wk_equal_bus_) wk_err("bus not created!");
        for (size_t i = 0; i < _wk_equal_bus_->n; i++) {
                WKEqualSlot *slot = wk_array_get(_wk_equal_bus_, i, WKEqualSlot *);
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
static int wk_int_compare(WKBox *a, WKBox *b) {
	int x, y;
	x = wk_box_get(a, int);
	y = wk_box_get(b, int);
	if (x < y) return -1;
	else if (x == y) return 0;
	else return 1;
}
static int wk_size_t_compare(WKBox *a, WKBox *b) {
	size_t x, y;
	x = wk_box_get(a, size_t);
	y = wk_box_get(b, size_t);
	if (x < y) return -1;
	else if (x == y) return 0;
	else return 1;
}
static int wk_float_compare(WKBox *a, WKBox *b) {
	float x, y;
	x = wk_box_get(a, float);
	y = wk_box_get(b, float);
	if (fabs(x - y) < wk_flt_eps) return 0;
	else if (x < y) return -1;
	else return 1;
}
static int wk_double_compare(WKBox *a, WKBox *b) {
	double x, y;
	x = wk_box_get(a, double);
	y = wk_box_get(b, double);
	if (fabs(x - y) < wk_dbl_eps) return 0;
	else if (x < y) return -1;
	else return 1;
}
static int wk_pointer_compare(WKBox *a, WKBox *b) {
	uintptr_t x, y;
	x = (uintptr_t)wk_box_get(a, size_t);
	y = (uintptr_t)wk_box_get(b, size_t);
	if (x < y) return -1;
	else if (x == y) return 0;
	else return 1;
}
void wk_compare_bus_init(void) {
	wk_compare_bus_connect("int", wk_int_compare);
	wk_compare_bus_connect("size_t", wk_size_t_compare);
	wk_compare_bus_connect("float", wk_float_compare);
	wk_compare_bus_connect("double", wk_double_compare);
	wk_compare_bus_connect("void *", wk_pointer_compare);
}
int wk_compare(WKBox *a, WKBox *b) {
	if (!_wk_compare_bus_) wk_err("bus not created!");
        for (size_t i = 0; i < _wk_compare_bus_->n; i++) {
                WKCompareSlot *slot = wk_array_get(_wk_compare_bus_, i, WKCompareSlot *);
		if (wk_box_is(a, slot->type) || wk_box_is(b, slot->type)) {
			return slot->action(a, b);
		}
        }
	wk_err("unknown type!");
        wk_fallback_with(-2);
}

/*----------------------------------------*/
/*           为总线注册一些常用             */
/*----------------------------------------*/
void wk_bus_init(void) {
	static bool first_run = true;
	if (first_run) {
		first_run = false;
		wk_free_bus_init();
		wk_hash_bus_init();
		wk_equal_bus_init();
		wk_compare_bus_init();
	}
}
