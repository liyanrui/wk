#ifndef WK_HASH_H
#define WK_HASH_H
#include <wk-bus.h>

typedef size_t (*WKHash)(WKBox *box);
wk_bus_declare(WKHash, wk_hash);

size_t wk_hash(WKBox *a);
void wk_hash_bus_connect(const char *type, WKHash action);

#endif
