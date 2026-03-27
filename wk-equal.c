#include "wk-equal.h"
wk_bus_define(WKEqual, wk_equal);

bool wk_equal(WKBox *a, WKBox *b) {
        if (!a || !b) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_equal_bus_->n; i++) {
                WKEqualSlot *slot = wk_array_get(_wk_equal_bus_, i, WKEqualSlot *);
                if (strcmp(slot->type, a->type) == 0
                    || strcmp(slot->type, a->type) == 0) {
                        bool result = false;
                        if(slot->action) result = slot->action(a, b);
                        return result;
                }
        }
        return false;
        wk_fallback_with(false);
}

