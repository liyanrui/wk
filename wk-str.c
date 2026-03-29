#include <stdint.h>
#include "wk-str.h"

/* 二阶多态函数所需 */
static size_t wk_raw_str_hash(const char *raw);
static size_t wk_str_hash(WKStr *str);
static bool wk_box_str_eq(WKBox *a, WKBox *b);

WKStr *wk_str(const char *raw) {
        /* 支持的总线函数 */
        bool first_run = true;
        if (first_run) {
                first_run = false;
                wk_free_bus_connect("WKStr *", (WKFree)wk_str_free);
                wk_hash_bus_connect("const char *", (WKHash)wk_raw_str_hash);
                wk_hash_bus_connect("WKStr *", (WKHash)wk_str_hash);
                /* 基于盒子的异构类型比较 */
                wk_equal_bus_connect("const char *", wk_box_str_eq);
                wk_equal_bus_connect("WKStr *", wk_box_str_eq);
        }
        size_t m, n;
        if (raw) {
                n = strlen(raw);
                m = n + 1;
        } else {
                n = 0;
                m = 1;
        }
        WKStr *str = malloc(sizeof(WKStr));
        char *body = malloc(m * sizeof(char));
        if (str && body) {
                if (raw) memcpy(body, raw, n);
                body[n] = '\0';
                str->n = n;
                str->m = m;
                str->body = body;
        } else {
                free(str);
                free(body);
                wk_err("failed to create WKStr object");
        }
        /* 正确与错误的返回点 */
        return str;
        wk_fallback_with(NULL);
}
void wk_str_free(WKStr *str) {
        if (!str) wk_err("invalid WKStr object");
        free(str->body);
        free(str);
        wk_fallback;
}

/* WKBox<WKStr *> */
WKStr *wk_v_str(const char *raw) {
        WKStr *str = wk_str(raw);
        return wk_v(str, WKStr *);
}

void wk_str_ins(WKStr *str, size_t index, const char *raw) {
        if (index > str->n) wk_err("overrange");
        if (!raw) wk_err("invalid raw string");
        /* 对 str 扩容 */
        size_t n = strlen(raw);
        size_t m = str->n + (n << 1); /* 扩容时，不妨多扩一些 */
        if (str->m < m) {
                char *new_body = realloc(str->body, m);
                if (!new_body) wk_err("out-of-memory");
                str->body = new_body;
                str->m = m;
        }
        size_t new_n = str->n + n;
        if ((str->n - index) > 0) {
                /* 将 str->body 的 [index, str->n) 部分移到尾部 */
                char *begin = str->body + index;
                memmove(begin + n, begin, str->n - index);
        }
        /* 将 raw 插入字符串对象的实体 */
        memcpy(str->body + index, raw, n);
        str->body[new_n] = '\0';
        str->n = new_n;
        wk_fallback;
}

void wk_str_prefix(WKStr *str, const char *raw) {
        wk_str_ins(str, 0, raw);
}

void wk_str_suffix(WKStr *str, const char *raw) {
        wk_str_ins(str, str->n, raw);
}

size_t wk_str_find(WKStr *str, const char *target) {
        if (!target) wk_err("invalid target");
        char *where = strstr(str->body, target);
        if (!where) wk_err("no such target");
        return where - str->body;
        wk_fallback_with(str->n + 1);
}

void wk_str_del(WKStr *str, size_t begin, size_t n) {
        /* 检查 [begin, begin + n] 是否超范围，或者 begin 为无穷大 */
        size_t j = begin + n;
        if (begin >= str->n || j > str->n || begin == SIZE_MAX) {
                wk_err("overrange");
        }
        if (n == 0) return;
        /* 用 begin + n 之后的内容补位 */
        memmove(str->body + begin, str->body + j, str->n - j);
        str->n -= n;
        str->body[str->n] = '\0';
        /* 缩容 */
        size_t new_m = str->n + 2 * n;
        if (new_m < str->m) {
                char *new_data = realloc(str->body, new_m);
                if (!new_data) wk_err("memory not enough");
                str->body = new_data;
                str->m = new_m;
        }
        wk_fallback;
}

WKStr *wk_str_trim(WKStr *str) {
        size_t begin = 0, n = 0;
        /* 去除首部空白字符 */
        for (const char *p = str->body; *p != '\0'; p++) {
                if (*p == ' ' || *p == '\t' || *p == '\n') n++;
                else break;
        }
        wk_str_del(str, begin, n);
        /* 去除尾部空白字符 */
        n = 0;
        for (const char *p = str->body + str->n - 1; p >= str->body; p--) {
                if (*p == ' ' || *p == '\t' || *p == '\n') n++;
                else break;
        }
        if (n > 0) {
                begin = str->n - n;
                wk_str_del(str, begin, n);
        }
        return str;
}

void wk_str_suffix_char(WKStr *str, char c) {
        char s[2] = {c, '\0'};
        wk_str_suffix(str, s);
}

WKStr *wk_str_from_file(const char *path) {
        FILE *f = fopen(path, "r");
        if (!f) goto ERROR;
        WKStr *buffer = wk_str(NULL);
        /* 将文件内容逐字符（char 类型）存入 buffer */
        int c;
        while ((c = fgetc(f)) != EOF) {
                wk_str_suffix_char(buffer, (char)c);
        }
        return buffer;
ERROR:
        fprintf(stderr, "%s: failed to open %s!\n", __func__, path);
        return NULL;
}

/* 二阶多态函数特化 */
/* 由于需要支持 WKStr 与 C 字符串的比较，故而只能在盒子层面实现 */
static bool wk_box_str_eq(WKBox *a, WKBox *b) {
        const char *x;
        if (strcmp(a->type, "WKStr *") == 0) {
                x = wk_box_get(a, WKStr *)->body;
        } else if (strcmp(a->type, "const char *") == 0) {
                x = wk_box_get(a, const char *);
        } else wk_err("invalid string object!");
        const char *y;
        if (strcmp(b->type, "WKStr *") == 0) {
                y = wk_box_get(b, WKStr *)->body;
        } else if (strcmp(b->type, "const char *") == 0) {
                y = wk_box_get(b, const char *);
        } else wk_err("invalid string object!");
        if (!x || !y) wk_err("invalid WKBox object");
        /* 比较 x 与 y */
        return strcmp(x, y) == 0 ? true : false;
        wk_fallback_with(false);
}

static size_t wk_raw_str_hash(const char *raw) {
        if (!raw) wk_err("invalid string");
        size_t hash = 5381;
        while (*raw != '\0') {
                hash = (hash << 5) + hash + *raw;
                raw++;
        }
        return hash;
        wk_fallback_with(0);
}

static size_t wk_str_hash(WKStr *str) {
        if (!str) wk_err("invalid WKStr object");
        return wk_raw_str_hash(str->body);
        wk_fallback_with(0);
}
