#ifndef WK_V_H
#define WK_V_H
#include <wk-err.h>
#include <wk-bus.h>

#define _wk_global_boxes_n_ 1024
extern size_t _wk_global_scope_;
extern WKArray *_wk_global_boxes_;

void *WK_V(void *obj, size_t n, const char *type);
#define wk_v(obj, T) WK_V(obj, sizeof(T), #T)

#define wk_v_begin \
        size_t _wk_local_scope_ = _wk_global_scope_

#define wk_v_end do { \
        /* 释放对象 */ \
        while (_wk_global_boxes_) { \
                wk_free(wk_array_get(_wk_global_boxes_, \
                                     _wk_global_scope_ - 1, \
                                     WKBox *)); \
                _wk_global_scope_--; \
                if (_wk_global_scope_ == _wk_local_scope_) break; \
        } \
        /* 缩容 */ \
        if (_wk_global_boxes_ \
            && _wk_global_boxes_->n > _wk_global_boxes_n_ \
            && _wk_global_scope_ < _wk_global_boxes_n_) { \
                while (_wk_global_boxes_->n > _wk_global_boxes_n_) { \
                        wk_array_del(_wk_global_boxes_, _wk_global_boxes_->n - 1); \
                } \
        } \
} while (0)

#define wk_v_ret(val) wk_v_end; return val;
#define wk_v_fallback wk_v_end; return; wk_err_report;
#define wk_v_fallback_with(val) wk_v_end; wk_err_report; return val;

#endif
