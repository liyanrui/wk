#ifndef WK_TABLE_H
#define WK_TABLE_H
#include <wk-hash.h>
#include <wk-equal.h>
#include <wk-free.h>
#include <wk-list.h>

typedef struct {
        WKBox *key;
        void *body;
} WKEntry;

typedef struct {
        size_t n; /* 条目数量（键值对数量） */
        size_t u; /* 条目的值部分的内存长度 */
        size_t m; /* 模数 */
        WKArray *body; /* 元素为桶，即 WKList 对象 */
} WKTable;

WKTable *WK_TABLE(size_t val_size);
#define wk_table(T) WK_TABLE(sizeof(T))

void WK_TABLE_ADD(WKTable *table, WKBox *key, void *value, size_t val_size);
#define wk_table_add(table, key, val, T) do { \
        T _tmp_val_ = val; \
        WK_TABLE_ADD(table, key, &_tmp_val_, sizeof(T)); \
} while (0)

void *WK_TABLE_QUERY(WKTable *table, WKBox *key, size_t val_size);
#define wk_table_query(table, key, T) \
        *(T*)WK_TABLE_QUERY(table, key, sizeof(T))

void wk_table_del(WKTable *table, WKBox *key);
void wk_table_free(WKTable *table);

#endif
