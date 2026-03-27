#include "wk-str.h"

/* map == into WKBox<int> */
bool int_equal(WKBox *a, WKBox *b) {
        int x = wk_box_get(a, int);
        int y = wk_box_get(b, int);
        return x == y ? true : false;
}

int main(void) {
        /* 整数比较 */
        wk_equal_bus_connect("int", int_equal);
        /* 引用，无需释放 */
        WKBox *u = wk_box_ref(42, int);
        WKBox *v = wk_box_ref(42, int);
        if (wk_equal(u, v)) printf("u = v, ok!\n");

        /* C 字符串与 WKStr 对象比较 */
        WKBox *x = wk_box_ref("hi", const char *);
        WKBox *y = wk_v_str("hi");
        if (wk_equal(x, y)) printf("x = y, ok!\n");
        
        /* 字符串比较：与二阶多态函数的连接已在构造函数里完成 */
        /* a, b, c 会函数结束时自动释放 */
        WKBox *a = wk_v_str("Hello");
        WKBox *b = wk_v_str("Hello"); 
        WKBox *c = wk_v_str("World");
        if (wk_equal(a, b)) printf("a = b, ok!\n");
        if (!wk_equal(a, c)) printf("a != c, ok!\n");
        
        wk_v_ret(0);
        wk_v_fallback_with(-1);
}
