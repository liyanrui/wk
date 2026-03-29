#include "wk-bus.h"

/* free 总线函数 */
wk_bus_define(WKFree, wk_free);
void wk_free(WKBox *a) {
        if (!a) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_free_bus_->n; i++) {
                WKFreeSlot *slot = wk_array_get(_wk_free_bus_, i, WKFreeSlot *);
                if (strcmp(slot->type, a->type) == 0) {
                        if(slot->action) {
                                slot->action(wk_box_get(a, void *));
                        }
                        wk_box_free(a);
                        return;
                }
        }
        wk_fallback;
}

/* hash 总线函数 */
wk_bus_define(WKHash, wk_hash);
size_t wk_hash(WKBox *a) {
        if (!a) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_hash_bus_->n; i++) {
                WKHashSlot *slot = wk_array_get(_wk_hash_bus_, i, WKHashSlot *);
                if (strcmp(slot->type, a->type) == 0) {
                        size_t result = 0;
                        if(slot->action) {
                                result = slot->action(wk_box_get(a, void *));
                        }
                        return result;
                }
        }
        wk_fallback_with(0);
}

/* equal 总线函数 */
/* 用户无法提供可以渗透盒子内部的函数，故而只能由用户提供在盒子层面的比较函数 */
wk_bus_define(WKEqual, wk_equal);
bool wk_equal(WKBox *a, WKBox *b) {
        if (!a || !b) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_equal_bus_->n; i++) {
                WKEqualSlot *slot = wk_array_get(_wk_equal_bus_, i, WKEqualSlot *);
                if (strcmp(slot->type, a->type) == 0
                    || strcmp(slot->type, b->type) == 0) {
                        bool result = false;
                        if(slot->action) {
                                result = slot->action(a, b);
                        }
                        return result;
                }
        }
        return false;
        wk_fallback_with(false);
}
