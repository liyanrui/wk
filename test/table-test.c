#include "wk-str.h"
#include "wk-table.h"

int main(void) {
        printf(">>> testing table ...\n");
        const size_t n = 7;
        char *keys[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
        char *values[] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
        WKTable *table = wk_table(const char *);
        for (size_t i = 0; i < n; i++) {
                wk_table_add(table,
                             wk_box(wk_str(keys[i]), WKStr *),
                             values[i], const char *);
        }
        /* 检索 */
        const char *v = wk_table_query(table, wk_box_ref("WED", const char *), const char *);
        WK_HOPE(v);
        WK_HOPE(strcmp(values[2], v) == 0);
        /* 删除 */
        wk_table_del(table, wk_box_ref("WED", const char *));
        /* 再检索：若是用 wk_table_query 会引发程序报错，继而崩溃, 需要用 WK_TABLE_QUERY*/
        /* v = wk_table_query(table, wk_box_ref("WED", const char *), const char *); */
        void *nothing = WK_TABLE_QUERY(table, wk_box_ref("WED", const char *), sizeof(const char *));
        WK_HOPE(!nothing);
        wk_table_free(table);
        
        printf("OK!\n");
        return 0;
}
