#include "wk-heap.h"

WKHeap *WK_HEAP(const char *key_type, const char *val_type, WKHeapType type) {
	WKHeap *heap = malloc(sizeof(WKHeap));
	if (!heap) wk_err("failed to create WKHeap object!");
	heap->type = type;
	heap->n = 0;
	heap->body = wk_array(WKPair *);
	/* 虚函数集 */
	if (key_type) {
		wk_bus_init();
		heap->cmp = wk_compare_bus_get(key_type);
		heap->key_free = wk_free_bus_get(key_type);	
	} else {
		heap->cmp = wk_compare;
		heap->key_free = wk_free;
	}
	if (val_type) {
		wk_bus_init();
		heap->val_free = wk_free_bus_get(val_type);
	} else heap->val_free = wk_free;
	return heap;
	wk_fallback_with(NULL);
}

void wk_heap_free(WKHeap *heap) {
	for (size_t i = 0; i < heap->n; i++) {
		WKPair *entry = wk_array_get(heap->body, i, WKPair *);
		heap->key_free(entry->x);
		wk_box_free(entry->y);
		wk_pair_free(entry);
	}
	wk_array_free(heap->body);
	free(heap);
}

void wk_heap_push(WKHeap *heap, WKPair *entry) {
	wk_array_add(heap->body, entry, WKPair *);
	/* 上浮 */
	size_t current = heap->n;
	while (current > 0) {
		size_t parent = (current - 1) / 2;
		WKPair *parent_entry = wk_array_get(heap->body, parent, WKPair *);
		int result = heap->cmp(entry->x, parent_entry->x);
		if (heap->type == WK_MAX_HEAP) {
			if (result == -1 || result == 0) break;
			else {
				wk_array_put(heap->body, current, parent_entry, WKPair *);
				current = parent;
			}
		} else if (heap->type == WK_MIN_HEAP) {
			if (result == 0 || result == 1) break;
			else {
				wk_array_put(heap->body, current, parent_entry, WKPair *);
				current = parent;
			}
			
		} else wk_err("unknown heap type!");
	}
	wk_array_put(heap->body, current, entry, WKPair *);
	heap->n++;
	wk_fallback;
}

WKPair *wk_heap_pop(WKHeap *heap) {
	/* 取堆顶元素 */
	WKPair *entry = wk_array_get(heap->body, 0, WKPair *);
	/* 以最后一个节点临时补位 */
	wk_array_put(heap->body,
		     0,
		     wk_array_get(heap->body, heap->n - 1, WKPair *),
		     WKPair *);
	heap->n--;
	if (heap->n == 0) return entry;
	/* 重构堆顶 */
	WKPair *top_entry = wk_array_get(heap->body, 0, WKPair *);
	size_t current = 0;
	size_t child = 1;
	WKPair *child_entry = NULL;
	while (child < heap->n) {
		size_t bro = child + 1;
		child_entry = wk_array_get(heap->body, child, WKPair *);
		if (bro < heap->n) {
			WKPair *b = wk_array_get(heap->body, bro, WKPair *);
			int result = heap->cmp(child_entry->x, b->x);
			if (heap->type == WK_MAX_HEAP) {
				if (result == -1) {
					child = bro;
					child_entry = b;
				};
			} else if (heap->type == WK_MIN_HEAP) {
				if (result == 1) {
					child = bro;
					child_entry = b;
				}
			} else wk_err("unknown heap type!");
		}
		int result = heap->cmp(top_entry->x, child_entry->x);
		if (heap->type == WK_MAX_HEAP) {
			if (result == 0 || result == 1) break;
		} else if (heap->type == WK_MIN_HEAP) {
			if (result == -1 || result == 0) break;
		} else wk_err("unknown heap type!");
		wk_array_put(heap->body, current, child_entry, WKPair *);
		current = child;
		child = 2 * child + 1;
	}
	wk_array_put(heap->body, current, top_entry, WKPair *);
	return entry;
	wk_fallback_with(NULL);
}

void wk_heap_entry_free(WKHeap *heap, WKPair *entry) {
	heap->key_free(entry->x);
	heap->val_free(entry->y);
	wk_pair_free(entry);
}
