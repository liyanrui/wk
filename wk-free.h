#ifndef WK_FREE_H
#define WK_FREE_H
#include <wk-bus.h>

typedef void (*WKFree)(WKBox *box);
wk_bus_declare(WKFree, wk_free);

void wk_free(WKBox *a);
void wk_free_bus_connect(const char *type, WKFree action);

#endif
