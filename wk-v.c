#include "wk-v.h"
size_t _wk_global_scope_ = 0;
WKArray *_wk_global_boxes_ = NULL;

void *WK_V(void *obj, size_t u, const char *type) {
        WKBox *box = WK_BOX(&obj, u, type);
        if (!_wk_global_boxes_) _wk_global_boxes_ = wk_array(WKBox *);
        if (_wk_global_scope_ < _wk_global_boxes_->n) {
                wk_array_put(_wk_global_boxes_, _wk_global_scope_, box, WKBox *);
        } else wk_array_add(_wk_global_boxes_, box, WKBox *);
        _wk_global_scope_++;
        return obj;
}
