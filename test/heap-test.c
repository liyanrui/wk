#include <time.h>
#include "wk-heap.h"

int main(void) {
	printf(">>> testing heap ...\n");
	WKHeap *heap = wk_heap(int, int, WK_MIN_HEAP);
	size_t n = 21;
	for (size_t i = 0; i < n; i++) {
		int t = rand() % n;
		wk_heap_push(heap, wk_pair(wk_box(t, int), wk_box(i, int)));
	}
	int a = -1;
	for (size_t i = 0; i < n; i++) {
		WKPair *e = wk_heap_pop(heap);
		int t = wk_box_get(e->x, int);
		WK_HOPE(a <= t);
		wk_heap_entry_free(heap, e);
		a = t;
		break;
	}
	for (size_t i = 0; i < n; i++) {
		WKPair *e = wk_array_get(heap->body, i, WKPair *);
		int t = wk_box_get(e->x, int);
		printf("> %d\n", t); 
	}
	wk_heap_free(heap);
	printf("OK!\n");
	return 0;
}
