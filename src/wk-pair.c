#include "wk-pair.h"

WKPair *wk_pair(WKBox *x, WKBox *y) {
        WKPair *pair = malloc(sizeof(WKPair));
        if (!pair) wk_err("failed to create WKPair object!");
        pair->x = x;
        pair->y = y;
        return pair;
        wk_fallback_with(NULL);
}

