#include <math.h>
#include "wk-list.h"
#include "wk-table.h"

static WKEntry *wk_entry(WKBox *key, void *data, size_t n) {
        WKEntry *entry = malloc(sizeof(WKEntry));
        if (!entry) wk_err("failed to create WKEntry object!");
        entry->key = key;
        entry->body = malloc(n);
        if (!entry->body) wk_err("failed to create WkEngry object!");
        memcpy(entry->body, data, n);
        return entry;
        wk_fallback_with(NULL);
}

static bool is_prime_number(size_t x)
{
        size_t k = sqrt(x);
        size_t i = 2;
        for (; i <= k; i++) {
                if (x % i == 0) break;
        }
        return (i > k);
}

/* 若 n 为散列表长度，则以下函数可以选出距离 n 最近的素数 */
static size_t nearest_prime_number(size_t n) {
        size_t m = 0;
        for (size_t i = n; i >= 1; i--) {
                if (is_prime_number(i)) {
                        m = i;
                        break;
                }
        }
        return m;
}

WKTable *WK_TABLE(size_t val_size) {
        WKTable *table = malloc(sizeof(WKTable));
        if (!table) wk_err("failed to create WKTable object!");
        table->n = 0;  /* 初始元素数量 */
        table->u = val_size; 
        table->m = 3;  /* 初始模数 */
        table->body = wk_array(WKList *);
        for (size_t i = 0; i < table->m; i++) {
                wk_array_add(table->body, WK_LIST(val_size), WKList *);
        }
        return table;
        wk_fallback_with(NULL);
}

static void table_resize(WKTable *table, size_t val_size) {
        /* 将桶位数量增大 1.5 倍，重新计算散列模数 */
        size_t n = table->body->n; 
        size_t new_n = n + (n >> 1); /* 增大 1.5 倍 */
        for (size_t i = n; i < new_n; i++) { /* 增加空桶位 */
                wk_array_add(table->body, WK_LIST(val_size), WKList *);
        }
        table->m = nearest_prime_number(new_n);
        /* 将表中原有条目重新插入散列表 */
        for (size_t i = 0; i < n; i++) {
                WKList *bucket = wk_array_get(table->body, i, WKList *);
                for (WKLink *it = bucket->head; it; ) {
                        WKLink *next = it->next;
                        WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                        size_t new_id = wk_hash(entry->key) % table->m;
                        if (new_id != i) {
                                WKList *other = wk_array_get(table->body, new_id, WKList *);
                                wk_list_suffix(other, entry, WKEntry *);
                                wk_list_del(bucket, it);
                        }
                        it = next;
                }
        }
}

void WK_TABLE_ADD(WKTable *table, WKBox *key, void *value, size_t val_size) {
        if (val_size != table->u) wk_err("value size not matched");
        /* 计算散列值和键位 */
        size_t i = wk_hash(key) % table->m;
        /* 构造条目 x */
        WKEntry *x = wk_entry(key, value, val_size);
        if (!x) return;
        /* 遍历 i 桶，判断其中是否存在键与 x 重复的条目 y */
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        WKLink *y_link = NULL;
        WKEntry *y = NULL;
        for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                if (!entry) wk_err("invalid WKEntry object in bucket!");
                if (wk_equal(x->key, entry->key)) {
                        y_link = it;
                        y = entry;
                        break;
                }
        }
        /* 处理键重复问题 */
        if (y_link && y) {
                /* 从i 桶中删除 y 所在节点 */
                wk_list_del(bucket, y_link);
                /* 释放 y */
                wk_free(y->key);
                free(y->body);
                free(y);
                /* 更新散列表中条目数量 */
                table->n--;
        }
        /* 将条目 x 对加入桶中 */
        wk_list_suffix(bucket, x, WKEntry *);
        /* 更新散列表中条目数量 */
        table->n++;
        /* 扩容 */
        if (table->n > table->body->n) table_resize(table, val_size);
        wk_fallback;
}

void *WK_TABLE_QUERY(WKTable *table, WKBox *key, size_t val_size) {
        if (!key) wk_err("invalid key!");
        if (val_size > table->u) wk_err("value size not matched");
        size_t i = wk_hash(key) % table->m;
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        void *target = NULL;
        for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                if (wk_equal(key, entry->key)) {
                        target = entry->body;
                        break;
                }
        }
        if (!target) wk_err("invalid value");
        return target;
        wk_fallback_with(NULL);
}

void wk_table_del(WKTable *table, WKBox *key) {
        if (!key) wk_err("invalid key!");
        size_t i = wk_hash(key) % table->m;
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        for (WKLink *it = bucket->head; it; it = it->next) {
                WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                if (wk_equal(key, entry->key)) {
                        wk_free(entry->key);
                        free(entry->body);
                        free(entry);
                        wk_list_del(bucket, it);
                        table->n--;
                        break;
                }
        }
        wk_fallback;
}

void wk_table_free(WKTable *table) {
        if (!table) wk_err("invalid WKTable object!");
        for (size_t i = 0; i < table->body->n; i++) {
                WKList *bucket = wk_array_get(table->body, i, WKList *);
                if (!bucket) wk_err("invalid bucket in WKTable object!");
                for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                        WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                        if (!entry) wk_err("invalid key-value in WKTable object!");
                        wk_free(entry->key);
                        free(entry->body);
                        free(entry);
                }
                wk_list_free(bucket);
        }
        wk_array_free(table->body);
        free(table);
        wk_fallback;
}
