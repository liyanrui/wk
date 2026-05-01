#ifndef WK_TABLE_H
#define WK_TABLE_H
#include <wk-pair.h>
#include <wk-list.h>

typedef struct {
        size_t n; /* 条目数量（键值对数量） */
        size_t m; /* 模数 */
        WKArray *body; /* 元素为桶，即 WKList 对象 */
	/* 虚函数集 */
	size_t (*hash)(WKBox *a);
	bool (*equal)(WKBox *a, WKBox *b);
	void (*key_free)(WKBox *a);
	void (*val_free)(WKBox *a);
} WKTable;

WKTable *WK_TABLE(const char *key_type, const char *val_type);
#define wk_table(KEY_T, VAL_T) WK_TABLE(#KEY_T, #VAL_T)
void wk_table_add(WKTable *table, WKPair *a);
WKBox *wk_table_query(WKTable *table, WKBox *key);
void wk_table_del(WKTable *table, WKBox *key);

void wk_table_entry_free(WKTable *table, WKPair *entry);
void wk_table_free(WKTable *table);

#endif
