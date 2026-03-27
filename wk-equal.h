#ifndef WK_EQUAL_H
#define WK_EQUAL_H
#include <wk-bus.h>

typedef bool (*WKEqual)(WKBox *a, WKBox *b);
wk_bus_declare(WKEqual, wk_equal);

bool wk_equal(WKBox *a, WKBox *b);
void wk_equal_bus_connect(const char *type, WKEqual action);
#endif
