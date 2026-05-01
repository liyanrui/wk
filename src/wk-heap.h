#ifndef WK_HEAP_H
#define WK_HEAP_H
#include <wk-pair.h>

typedef enum {
	WK_MAX_HEAP, /* 大根堆 */
	WK_MIN_HEAP  /* 小根堆 */
} WKHeapType;

typedef struct {
	WKHeapType type;
	size_t n; /* n 个元素 */
	WKArray *body;
	/* 虚函数集 */
	int (*cmp)(WKBox *a, WKBox *b);
	void (*key_free)(WKBox *a);
	void (*val_free)(WKBox *a);
} WKHeap;

WKHeap *WK_HEAP(const char *key_type, const char *val_type, WKHeapType type);
#define wk_heap(KEY_T, VAL_T, HEAP_T) WK_HEAP(#KEY_T, #VAL_T, HEAP_T)
void wk_heap_free(WKHeap *heap);

void wk_heap_push(WKHeap *heap, WKPair *entry);
WKPair *wk_heap_pop(WKHeap *heap);
void wk_heap_entry_free(WKHeap *heap, WKPair *entry);

#endif
