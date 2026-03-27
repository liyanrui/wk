#ifndef WK_ERR_H
#define WK_ERR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern const char *_wk_err_;

/* 当 E 为 NULL 时，可抑制错误信息 */
#define wk_err(E) do { \
        _wk_err_ = E; \
        goto _WK_ERROR_; \
} while (0)

#define wk_err_report \
_WK_ERROR_: \
        if (_wk_err_) { \
                fprintf(stderr, "%s: %s!\n", __func__, _wk_err_); \
                _wk_err_ = NULL; \
        }

#define wk_fallback return; wk_err_report;
#define wk_fallback_with(val) wk_err_report; return val;

/* for writing test cases */
#define WK_HOPE(expr) do { \
        if (!(expr)) { \
                fprintf(stderr, "line %d: failed!\n", __LINE__); \
                exit(-1); \
        } \
} while (0)

#endif
