#include "wk-list.h"
#include "wk-table.h"

static bool is_prime_number(size_t x)
{
        /* 用 i^2 避免调用 math 库里的 sqrt */
        size_t i = 2;
        for (; i * i <= x; i++) {
                if (x % i == 0) break;
        }
        return (i * i > x);
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

WKTable *WK_TABLE(const char *key_type, const char *val_type) {
        WKTable *table = malloc(sizeof(WKTable));
        if (!table) wk_err("failed to create WKTable object!");
        table->n = 0;  /* 初始元素数量 */
        table->m = 3;  /* 初始模数 */
        table->body = wk_array(WKList *);
        for (size_t i = 0; i < table->m; i++) {
                wk_array_add(table->body, wk_list(WKBox *), WKList *);
        }
	/* 虚函数集 */
	table->hash = wk_hash_bus_get(key_type);
	if (!table->hash) table->hash = wk_hash;
	table->equal = wk_equal_bus_get(key_type);
	if (!table->equal) table->equal = wk_equal;
	table->key_free = wk_free_bus_get(key_type);
	if (!table->key_free) table->key_free = wk_free;
	table->val_free = wk_free_bus_get(val_type);
	if (!table->val_free) table->val_free = wk_free;
        return table;
        wk_fallback_with(NULL);
}

void wk_table_entry_free(WKTable *table, WKPair *entry) {
	table->key_free(entry->x);
	table->val_free(entry->y);
	wk_pair_free(entry);
}

void wk_table_free(WKTable *table) {
        if (!table) wk_err("invalid WKTable object!");
        for (size_t i = 0; i < table->body->n; i++) {
                WKList *bucket = wk_array_get(table->body, i, WKList *);
                if (!bucket) wk_err("invalid bucket in WKTable object!");
                for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                        WKPair *entry = wk_link_get(it, WKPair *);
                        if (!entry) wk_err("invalid key-value in WKTable object!");
			wk_table_entry_free(table, entry);
                }
                wk_list_free(bucket);
        }
        wk_array_free(table->body);
        free(table);
        wk_fallback;
}

static void table_resize(WKTable *table) {
        /* 将桶位数量增大 1.5 倍，重新计算散列模数 */
        size_t n = table->body->n; 
        size_t new_n = n + (n >> 1); /* 增大 1.5 倍 */
        for (size_t i = n; i < new_n; i++) { /* 增加空桶位 */
                wk_array_add(table->body, wk_list(WKBox *), WKList *);
        }
        table->m = nearest_prime_number(new_n);
        /* 将表中原有条目重新插入散列表 */
        for (size_t i = 0; i < n; i++) {
                WKList *bucket = wk_array_get(table->body, i, WKList *);
                for (WKLink *it = bucket->head; it; ) {
                        WKLink *next = it->next;
                        WKPair *entry = wk_link_get(it, WKPair *);
                        size_t new_id = table->hash(entry->x) % table->m;
                        if (new_id != i) {
                                WKList *other = wk_array_get(table->body, new_id, WKList *);
                                wk_list_suffix(other, entry, WKPair *);
                                wk_list_del(bucket, it);
                        }
                        it = next;
                }
        }
}

void wk_table_add(WKTable *table, WKPair *a) {
        /* 计算散列值和键位 */
        size_t i = table->hash(a->x) % table->m;
        /* 遍历 i 桶，判断其中是否存在键与 a->x 重复的条目 y */
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        WKLink *b_link = NULL;
        WKPair *b = NULL;
        for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                WKPair *entry = wk_link_get(it, WKPair *);
                if (!entry) wk_err("invalid WKPair object in bucket!");
                if (table->equal(a->x, entry->x)) {
                        b_link = it;
                        b = entry;
                        break;
                }
        }
        /* 处理键重复问题 */
        if (b_link && b) {
                /* 从i 桶中删除 b 所在节点 */
                wk_list_del(bucket, b_link);
		wk_table_entry_free(table, b);
                /* 更新散列表中条目数量 */
                table->n--;
        }
        /* 将条目 a 对加入桶中 */
        wk_list_suffix(bucket, a, WKPair *);
        /* 更新散列表中条目数量 */
        table->n++;
        /* 扩容 */
        if (table->n > table->body->n) table_resize(table);
        wk_fallback;
}

WKBox *wk_table_query(WKTable *table, WKBox *key) {
	/* 检索过程为了更为稳健（例如兼容 WKStr * 和 const char *），使用二阶函数 */
        size_t i = wk_hash(key) % table->m;
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        WKBox *target = NULL;
        for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                WKPair *entry = wk_link_get(it, WKPair *);
                if (table->equal(key, entry->x)) {
                        target = entry->y;
                        break;
                }
        }
        return target;
}

void wk_table_del(WKTable *table, WKBox *key) {
        size_t i = table->hash(key) % table->m;
        WKList *bucket = wk_array_get(table->body, i, WKList *);
        for (WKLink *it = bucket->head; it; it = it->next) {
                WKPair *entry = wk_link_get(it, WKPair *);
                if (table->equal(key, entry->x)) {
			wk_table_entry_free(table, entry);
                        wk_list_del(bucket, it);
                        table->n--;
                        break;
                }
        }
}
