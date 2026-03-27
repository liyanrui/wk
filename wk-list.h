#ifndef WK_LIST_H
#define WK_LIST_H
#include <stdio.h>

typedef struct wk_link {
        void *body;
        struct wk_link *prev;
        struct wk_link *next;
} WKLink;

typedef struct wk_list {
        size_t n; /* 节点数量 */
        size_t u; /* 链表节点所存数据的长度（字节数） */
        WKLink *head;
        WKLink *tail;
} WKList;

WKList *WK_LIST(size_t u);
#define wk_list(T) WK_LIST(sizeof(T))

void wk_list_free(WKList *list);

WKLink *WK_LIST_INSF(WKList *list, WKLink *here, void *data, size_t u);
WKLink *WK_LIST_INSB(WKList *list, WKLink *here, void *data, size_t u);
#define wk_list_prefix(list, val, T) WK_LIST_INSF(list, list->head, &(T){val}, sizeof(T))
#define wk_list_suffix(list, val, T) WK_LIST_INSB(list, list->tail, &(T){val}, sizeof(T))
void wk_list_del(WKList *list, WKLink *target);

void *WK_LIST_GET(WKList *list, WKLink *link, size_t u);
#define wk_list_get(list, link, T) \
        *(T*)WK_LIST_GET(list, link, sizeof(T))

#endif
