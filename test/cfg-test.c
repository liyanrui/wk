#include "wk-cfg.h"

int main(void) {
        printf(">>> testing cfg ...\n");
        WKTable *cfg = wk_cfg("demo.cfg");
        const char *keys[] = {"左值", "姓名", "\"籍贯\"", "政治面貌"};
        WKBox *k; WKStr *v;
        k =  wk_box_ref(keys[0], const char *);
        v = wk_table_query(cfg, k, WKStr *);
        WK_HOPE(strcmp(v->body, "右值") == 0);
        k =  wk_box_ref(keys[1], const char *);
        v = wk_table_query(cfg, k, WKStr *);
        WK_HOPE(strcmp(v->body, "\"李缓慢\"") == 0);
        k =  wk_box_ref(keys[2], const char *);
        v = wk_table_query(cfg, k, WKStr *);
        WK_HOPE(strcmp(v->body, "淮海省") == 0);
        k =  wk_box_ref(keys[3], const char *);
        v = wk_table_query(cfg, k, WKStr *);
        WK_HOPE(strcmp(v->body, "群\"HELLO\"众") == 0);
        wk_cfg_free(cfg);
        printf("OK!\n");
        return 0;
}
