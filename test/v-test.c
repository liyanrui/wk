#include "wk-str.h"

int foo(void *data) {
        wk_v_begin; 
        if (!data) wk_err("invalid data!");
        for (size_t i = 0; i < 10; i++) {
                wk_str_v("Hello world!");
        }
        wk_v_ret(42); 
        wk_v_fallback_with(-1); 
}

int main(void) {
	printf("testing v!\n");
        double pi = 3.14;
        WK_HOPE(foo(&pi) == 42);
        printf("preparing to fail!\n");
        WK_HOPE(foo(NULL) == -1);
        printf("OK!\n");
        return 0;
}
