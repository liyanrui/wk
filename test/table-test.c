#include "wk-table.h"

int main(void) {
        printf(">>> testing table ...\n");
        const size_t n = 7;
        char *keys[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
        char *values[] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
        WKTable *table = wk_table(WKStr *, const char *);
        for (size_t i = 0; i < n; i++) {
                wk_table_add(table,
                             wk_pair(wk_box(wk_str(keys[i]), WKStr *),
				     wk_box(values[i], const char *)));
        }
        /* 检索 */
        const char *t = wk_box_get(wk_table_query(table, wk_box_ref("WED", const char *)),
				   const char *);
        WK_HOPE(t);
        WK_HOPE(strcmp(values[2], t) == 0);
        /* 删除 */
        wk_table_del(table, wk_box_ref("WED", const char *));
        /* 再检索 */
        WKBox *v = wk_table_query(table, wk_box_ref("WED", const char *));
        WK_HOPE(!v);
        wk_table_free(table);
        
        printf("OK!\n");
        return 0;
}
