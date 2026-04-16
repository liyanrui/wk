#ifndef WK_STR_H
#define WK_STR_H
#include <wk-v.h>

typedef struct {
        size_t n;    /* 字符串长度 */
        size_t m;    /* 字符串空间长度 */
        char *body;  /* 字符串空间 */
} WKStr;

WKStr *wk_str(const char *raw);
WKStr *wk_str_v(const char *raw);
void wk_str_free(WKStr *str);

void wk_str_ins(WKStr *str, size_t index, const char *raw);
#define wk_str_prefix(str, raw) wk_str_ins(str, 0, raw)
#define wk_str_suffix(str, raw) wk_str_ins(str, str->n, raw)

void wk_str_suffix_char(WKStr *str, char c);
WKStr *wk_str_from_file(const char *path);

size_t wk_str_find(WKStr *str, const char *target);
void wk_str_del(WKStr *str, size_t begin, size_t n);
WKStr *wk_str_trim(WKStr *str);

#endif
