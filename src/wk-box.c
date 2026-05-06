#include "wk-err.h"
#include "wk-box.h"

WKBox *WK_BOX(void *data, size_t n, const char *type) {
        WKBox *box = malloc(sizeof(WKBox) + n);
        if (!box) wk_err("failed to create WKBox object");
        box->type = type;
        box->is_ref = false;
	box->body = malloc(n);
	if (!box->body) {
		free(box);
		wk_err("failed to create body of WKBox object");
	}
        memcpy(box->body, data, n);
        return box;
        wk_fallback_with(NULL);
}

void wk_box_free(WKBox *box) {
        if (box->is_ref) return;
	free(box->body);
        free(box);
}
