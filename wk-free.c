#include "wk-free.h"
wk_bus_define(WKFree, wk_free);

void wk_free(WKBox *a) {
        if (!a) wk_err("invalid WKBox object");
        for (size_t i = 0; i < _wk_free_bus_->n; i++) {
                WKFreeSlot *slot = wk_array_get(_wk_free_bus_, i, WKFreeSlot *);
                if (strcmp(slot->type, a->type) == 0) {
                        if(slot->action) slot->action(a);
                        wk_box_free(a);
                        return;
                }
        }
        wk_fallback;
}
