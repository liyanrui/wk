#include <stdint.h>
#include <stdarg.h>
#include "wk-str.h"

WKStr *wk_str(const char *raw) {
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
        free(str->body);
        free(str);
}

void wk_str_ins(WKStr *str, size_t index, const char *raw) {
        if (index > str->n) wk_err("overrange");
        /* 对 str 扩容 */
        size_t n = strlen(raw);
	if (n == 0) return;
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

size_t wk_str_find(WKStr *str, const char *target) {
        char *where = strstr(str->body, target);
	return where ? where - str->body : str->n;
}

void wk_str_del(WKStr *str, size_t begin, size_t n) {
        /* 检查 [begin, begin + n] 是否超范围，或者 begin 为无穷大 */
        size_t j = begin + n;
        if (begin >= str->n || j > str->n || begin == SIZE_MAX) {
                wk_err("overrange");
        }
        /* 用 begin + n 之后的内容补位 */
        memmove(str->body + begin, str->body + j, str->n - j);
        str->n -= n;
        /* /\* 缩容 *\/ */
        /* size_t new_m = (str->n << 1) + 1; /\* 留出 \0 位 *\/ */
        /* if (new_m < str->m) { */
        /*         char *new_data = realloc(str->body, new_m); */
        /*         if (!new_data) wk_err("memory not enough"); */
        /*         str->body = new_data; */
        /*         str->m = new_m; */
        /* } */
	str->body[str->n] = '\0';
        wk_fallback;
}

WKStr *wk_str_trim(WKStr *str) {
	if (str->n == 0) return str;
        size_t begin = 0, n = 0;
        /* 去除首部空白字符 */
        for (const char *p = str->body; *p != '\0'; p++) {
                if (*p == ' ' || *p == '\t' || *p == '\n') n++;
                else break;
        }
        wk_str_del(str, begin, n);
	if (str->n == 0) return str;
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

/* 在 str 里查找与 a 相同的子字符串，并将其替换为 b */ 
void wk_str_replace(WKStr *str, const char *a, const char *b) {
	size_t n = strlen(a);
	WKStr *new_str = wk_str(NULL);
	const char *a1 = str->body;
	const char *a2 = NULL;
	while (1) {
		a2 = strstr(a1, a);
		if (a2) {
			for (const char *q = a1; q < a2; q++) {
				wk_str_suffix_char(new_str, *q);
			}
			wk_str_suffix(new_str, b);
		} else {
			/* 加入余下部分 */
			wk_str_suffix(new_str, a1);
			break;
		}
		a2 += n;
		a1 = a2;
	}
	free(str->body);
	str->body = new_str->body;
	str->n = new_str->n;
	str->m = new_str->m;
	free(new_str);
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
	fclose(f);
        return buffer;
ERROR:
        fprintf(stderr, "%s: failed to open %s!\n", __func__, path);
        return NULL;
}

void wk_str_printf(WKStr *str, const char *fmt, ...) {
	va_list args1, args2;
	int n;
	va_start(args1, fmt);
	va_copy(args2, args1);
	n = vsnprintf(NULL, 0, fmt, args1);
	if (str->m < n + 1) {
		str->m += (n + 1);
		char *new_body = realloc(str->body, str->m);
		if (!new_body) wk_err("out-of-memory");
                str->body = new_body;
	}
	vsnprintf(str->body, n + 1, fmt, args2);
	va_end(args2);
	va_end(args1);
	str->n = n;
	wk_fallback;
}
