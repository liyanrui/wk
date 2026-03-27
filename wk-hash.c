#include "wk-hash.h"
wk_bus_define(WKHash, wk_hash);

size_t wk_hash(WKBox *a) {
        if (!a) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_hash_bus_->n; i++) {
                WKHashSlot *slot = wk_array_get(_wk_hash_bus_, i, WKHashSlot *);
                if (strcmp(slot->type, a->type) == 0) {
                        size_t result = 0;
                        if(slot->action) result = slot->action(a);
                        return result;
                }
        }
        wk_fallback_with(0);
}
