#include <string.h>
#include "wk-err.h"
#include "wk-list.h"

WKList *WK_LIST(size_t u) {
        WKList *list = malloc(sizeof(WKList));
        if (!list) wk_err("failed to create WKList object");
        list->u = u;
        list->n = 0;
        list->head = NULL;
        list->tail = NULL;
        return list;
        wk_fallback_with(NULL);
}

void wk_list_free(WKList *list) {
        WKLink *it = list->head;
        while (it) {
                WKLink *next = it->next;
                free(it->body);
                free(it);
                it = next;
        }
        free(list);
}

static WKLink *wk_link(void *data, size_t u) {
        WKLink *new_link = malloc(sizeof(WKLink));
        if (!new_link) wk_err("memory not enough");
        new_link->prev = NULL;
        new_link->next = NULL;
        new_link->body = malloc(u);
        if (!new_link->body) wk_err("memory not enough");
        memcpy(new_link->body, data, u);
        return new_link;
        wk_fallback_with(NULL);
}

WKLink *WK_LIST_INSF(WKList *list, WKLink *here, void *data, size_t u) {
        if (list->u != u) wk_err("size not matched");
        WKLink *new_link = wk_link(data, u);
        if (here) {
                new_link->prev = here->prev;
                new_link->next = here;
                here->prev = new_link;
        } else {
                if (list->n == 0) { /* 仅当 list 为空表时允许 here 为空 */
                        list->head = new_link;
                        list->tail = new_link;
                } else wk_err("invalid link");
        }
        if (!new_link->prev) list->head = new_link;
        list->n++;
        return new_link;
        wk_fallback_with(NULL);
}

WKLink *WK_LIST_INSB(WKList *list, WKLink *here, void *data, size_t u) {
        if (list->u != u) wk_err("size not matched");
        WKLink *new_link = wk_link(data, u);
        if (here) {
                new_link->prev = here;
                new_link->next = here->next;
                here->next = new_link;
        } else {
                if (list->n == 0) { /* 仅当 list 为空表时允许 here 为空 */
                        list->head = new_link;
                        list->tail = new_link;
                } else wk_err("invalid link");
        }
        if (!new_link->next) list->tail = new_link;
        list->n++;
        return new_link;
        wk_fallback_with(NULL);
}

void wk_list_del(WKList *list, WKLink *target) {
        WKLink *prev = target->prev;
        WKLink *next = target->next;
        if (prev) prev->next = next;
        else list->head = next;
        if (next) next->prev = prev;
        else list->tail = prev;
        free(target->body);
        free(target);
        list->n--;
}
