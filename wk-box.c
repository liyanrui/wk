#include "wk-err.h"
#include "wk-box.h"

WKBox *WK_BOX(void *data, size_t size, const char *type) {
        WKBox *box = malloc(sizeof(WKBox));
        if (!box) wk_err("failed to create WKBox object");
        box->type = type;
        box->size = size;
        box->is_ref = false;
        box->body = malloc(size);
        if (!box->body) {
                free(box);
                wk_err("failed to create WKBox object");
        }
        memcpy(box->body, data, size);
        return box;
        wk_fallback_with(NULL);
}

void WK_BOX_PUT(WKBox *box, void *data, size_t size, const char *type) {
        if (!box) wk_err("invalid WKBox object");
        if (!data || size == 0 || !type) wk_err("invalid object");
        if (box->size < size) {
                void *new_body = realloc(box->body, size);
                if (!new_body) wk_err("memory not enough");
                box->body = new_body;
        }
        memcpy(box->body, data, size); 
        box->type = type;
        box->size = size;
        wk_fallback;
}

void wk_box_free(WKBox *box) {
        if (!box) wk_err("invalid WKBox object");
        if (box->is_ref) return;
        free(box->body);
        free(box);
        wk_fallback;
}

void *WK_BOX_GET(WKBox *box, size_t size, const char *type) {
        if (!box) wk_err("invalid WKBox object");
        /* 类型检测 */
        if (!wk_box_is(box, type) && !wk_box_is(box, "void *")) {
                wk_err("type not matched");
        }
        if (box->size < size) { /* 取数据时，长度可以小于盒子里数据的长度 */
                wk_err("size not matched");
        }
        if (!box->body) wk_err("invalid value");
        return box->body;
        wk_fallback_with(NULL);
}

