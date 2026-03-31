#ifndef WK_CFG_H
#define WK_CFG_H
#include <wk-str.h>
#include <wk-table.h>

WKTable *wk_cfg(const char *path);
void wk_cfg_free(WKTable *cfg);

#endif
