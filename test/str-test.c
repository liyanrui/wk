#include <wk-str.h>
int main(void) {
        printf(">>> testing str...\n");
        WKStr *a = wk_str(NULL);
        WK_HOPE(strcmp(a->body, "") == 0);
        wk_str_suffix(a, "world");
        wk_str_prefix(a, "hello");
        WK_HOPE(strcmp(a->body, "helloworld") == 0);

        const char *udl = "_____";
        wk_str_ins(a, 4, udl);
        wk_str_suffix_char(a, '!');
        WK_HOPE(strcmp(a->body, "hell_____oworld!") == 0);
        wk_str_del(a, wk_str_find(a, udl), strlen(udl));
        WK_HOPE(strcmp(a->body, "helloworld!") == 0);
        wk_str_del(a, strlen("hello"), 1);
        WK_HOPE(strcmp(a->body, "helloorld!") == 0);
        wk_str_del(a, 0, a->n);
        WK_HOPE(strcmp(a->body, "") == 0);
        WK_HOPE(a->n == 0);
        wk_str_free(a);
        
        printf("OK!\n");
        return 0;
}
