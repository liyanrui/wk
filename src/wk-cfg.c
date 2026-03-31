#include "wk-cfg.h"

typedef enum {
        PLAIN_LEFT_V,
        PLAIN_RIGHT_V,
        QUOTED_LEFT_V,
        QUOTED_RIGHT_V
} TokenType;

typedef struct {
        TokenType type;
        WKStr *content;
        size_t line;
} Token;

Token *create_token(TokenType type, size_t line, WKStr *content) {
        Token *t = malloc(sizeof(Token));
        if (!t) wk_err("failed to create Token object!");
        t->type = type;
        t->line = line;
        t->content = content;
        return t;
        wk_fallback_with(NULL);
}

void tokens_free(WKList *tokens) {
        for (WKLink *it = tokens->head; it; it = it->next) {
                Token *t = wk_list_get(tokens, it, Token *);
                if (t) {
                        if (t->content) wk_str_free(t->content);
                        free(t);
                }
        }
        wk_list_free(tokens);  
}

WKTable *wk_cfg(const char *path) {
        FILE *fp = fopen(path, "r");
        if (!fp) {
                fprintf(stderr, "failed to open %s", path);
                goto ERROR;
        }
        WKList *tokens = wk_list(Token *);
        enum {INIT,
              LEFT_V, RIGHT_V,
              Q_LEFT_V, Q_LEFT_SKIP, Q_LEFT_V_END,
              COLON,
              Q_RIGHT_V, Q_RIGHT_SKIP,
              COMMENT,
              ILLEGAL} status = INIT;
        int c, before_c, after_c;
        size_t line = 1;
        WKStr *value = wk_str(NULL);
        while ((c = fgetc(fp)) != EOF) {
                switch (status) {
                case INIT:
                        if (c == ' ' || c == '\t' || c == '\n') ;
                        else if (c == '\"') status = Q_LEFT_V;
                        else if (c == '#') status = COMMENT;
                        else if (c == ':') status = ILLEGAL;
                        else {
                                wk_str_suffix_char(value, c);
                                status = LEFT_V;
                        }
                        break;
                case LEFT_V:
                        if (c == '\"') {
                                fseek(fp, -2, SEEK_CUR);
                                before_c = fgetc(fp);
                                fseek(fp, 1, SEEK_CUR);
                                if (before_c == '\\') { /* 转义引号 */
                                        /* 删除转义符 */
                                        wk_str_del(value, value->n - 1, 1);
                                        /* 添加引号 */
                                        wk_str_suffix_char(value, c);
                                } else status = ILLEGAL;
                        } else if (c ==  ':') { /* 构造左值 */
                                Token *t = create_token(PLAIN_LEFT_V, line, wk_str_trim(value));
                                wk_list_suffix(tokens, t, Token *);
                                value = wk_str(NULL);
                                status = COLON;
                        } else if (c == '#') {
                                status = ILLEGAL;
                        } else if (c == '\n') {
                                status = ILLEGAL;
                        } else {
                                wk_str_suffix_char(value, c);
                        }
                        break;
                case RIGHT_V:
                        if (c == '\"') {
                                fseek(fp, -2, SEEK_CUR);
                                before_c = fgetc(fp);
                                fseek(fp, 1, SEEK_CUR);
                                if (before_c == '\\') { /* 转义引号 */
                                        /* 删除转义 */
                                        wk_str_del(value, value->n - 1, 1);
                                        /* 添加引号 */
                                        wk_str_suffix_char(value, c);
                                } else status = ILLEGAL;
                        } else if (c == '\n') { /* 构造右值 */
                                Token *t = create_token(PLAIN_RIGHT_V, line, wk_str_trim(value));
                                wk_list_suffix(tokens, t, Token *);
                                value = wk_str(NULL);
                                status = INIT;
                        } else if (c == '#') { /* 构造右值 */
                                Token *t = create_token(PLAIN_RIGHT_V, line, wk_str_trim(value));
                                wk_list_suffix(tokens, t, Token *);
                                value = wk_str(NULL);
                                status = COMMENT;
                        } else if (c == ':') {
                                status = ILLEGAL;
                        } else {
                                wk_str_suffix_char(value, c);
                                after_c = fgetc(fp);
                                fseek(fp, -1, SEEK_CUR);
                                if (after_c == EOF) { /* 若文件最后一个字符并非换行符，而此时应当收集右值了 */
                                        Token *t = create_token(PLAIN_RIGHT_V, line, wk_str_trim(value));
                                        wk_list_suffix(tokens, t, Token *);
                                        value = NULL;
                                }
                        }
                        break;
                case Q_LEFT_V:
                        if (c == '\"') {
                                /* 判断是否为转义引号 */
                                fseek(fp, -2, SEEK_CUR);
                                before_c = fgetc(fp);
                                fseek(fp, 1, SEEK_CUR);
                                if (before_c != '\\') {
                                        Token *t = create_token(QUOTED_LEFT_V, line, wk_str_trim(value));
                                        wk_list_suffix(tokens, t, Token *);
                                        value = wk_str(NULL);
                                        status = Q_LEFT_V_END;
                                } else {
                                        /* 删除转义符 */
                                        wk_str_del(value, value->n - 1, 1);
                                        /* 添加引号 */
                                        wk_str_suffix_char(value, c);
                                }
                        } else if (c == '\n') {
                                status = Q_LEFT_SKIP;
                        } else {
                                wk_str_suffix_char(value, c);
                        }
                        break;
                case Q_LEFT_SKIP:
                        if (c == ' ' || c == '\t' || c == '\n') ;
                        else {
                                fseek(fp, -1, SEEK_CUR);
                                status = Q_LEFT_V;
                        }
                        break;
                case Q_LEFT_V_END:
                        if (c == ':') status = COLON;
                        else if (c == '\n') status = ILLEGAL;
                        else if (c == ' ' || c == '\t') ;
                        else status = ILLEGAL;
                        break;
                case COLON:
                        if (c == '\"') {
                                /* 判断是否为转义引号 */
                                fseek(fp, -2, SEEK_CUR);
                                before_c = fgetc(fp);
                                fseek(fp, 1, SEEK_CUR);
                                if (before_c == '\\') {
                                        /* 删除转义符 */
                                        wk_str_del(value, value->n - 1, 1);
                                        /* 添加引号 */
                                        wk_str_suffix_char(value, c);
                                        status = RIGHT_V;
                                } else status = Q_RIGHT_V;
                        }
                        else if (c == '\n') status = ILLEGAL;
                        else if (c == ' ' || c == '\t') ;
                        else if (c == '#') {
                                status = ILLEGAL;
                        } else {
                                wk_str_suffix_char(value, c);
                                status = RIGHT_V;
                        }
                        break;
                case Q_RIGHT_V:
                        if (c == '\"') {
                                /* 判断是否为转义引号 */
                                fseek(fp, -2, SEEK_CUR);
                                before_c = fgetc(fp);
                                fseek(fp, 1, SEEK_CUR);
                                if (before_c != '\\') {
                                        Token *t = create_token(QUOTED_RIGHT_V, line, wk_str_trim(value));
                                        wk_list_suffix(tokens, t, Token *);
                                        value = wk_str(NULL);
                                        status = INIT;
                                } else {
                                        /* 删除转义符 */
                                        wk_str_del(value, value->n - 1, 1);
                                        /* 添加引号 */
                                        wk_str_suffix_char(value, c);
                                }
                        } else if (c == '\n') status = Q_RIGHT_SKIP;
                        else wk_str_suffix_char(value, c);
                        break;
                case Q_RIGHT_SKIP:
                        if (c == ' ' || c == '\t' || c == '\n') ;
                        else {
                                fseek(fp, -1, SEEK_CUR);
                                status = Q_RIGHT_V;
                        }
                        break;
                case COMMENT:
                        if (c == '\n') status = INIT;
                        break;
                case ILLEGAL:
                        fprintf(stderr, "line %lu error: %s", line, value->body);
                        goto STOP;
                default:
                        fprintf(stderr, "unknown token type!");
                        goto STOP;
                }
                if (c == '\n') line++;
        }
STOP:
        if (value) wk_str_free(value);
        /* 将 tokens 中的左值和右值存入词典 */
        WKTable *cfg = NULL;
        if (tokens) {
                WKLink *it = tokens->head;
                cfg = wk_table(WKStr *);
                while (it) {
                        Token *a = wk_list_get(tokens, it, Token *);
                        if (it->next) {
                                Token *b = wk_list_get(tokens, it->next, Token *);
                                if ((a->type == PLAIN_LEFT_V || a->type == QUOTED_LEFT_V)
                                    && (b->type == PLAIN_RIGHT_V || b->type == QUOTED_RIGHT_V)) {
                                        WKStr *v = b->content;
                                        wk_table_add(cfg, wk_box(a->content, WKStr *), v, WKStr *);
                                        a->content = NULL; /* 所有权已转移至 cfg */
                                        b->content = NULL; /* 所有权已转移至 cfg */
                                }
                                it = it->next->next;
                        } else break;
                }
                tokens_free(tokens);
        }
        return cfg;
ERROR:
        return NULL;
}

void wk_cfg_free(WKTable *cfg) {
        if (!cfg) wk_err("invalid configuation!");
        for (size_t i = 0; i < cfg->body->n; i++) {
                WKList *bucket = wk_array_get(cfg->body, i, WKList *);
                if (!bucket) wk_err("invalid configuation!");
                for (WKLink *it = bucket->head; it != NULL; it = it->next) {
                        WKEntry *entry = wk_list_get(bucket, it, WKEntry *);
                        if (!entry) wk_err("invalid key-value in WKTable object!");
                        WKStr *val = *(WKStr **)entry->body; /* 注意，table 里存储的是值的地址！ */
                        wk_str_free(val);
                }
        }
        wk_table_free(cfg);
        wk_fallback;
}
