#include <string.h>
#include "wk-cli.h"

static double opt_match(const char *a, const char *b) {
        size_t na = strlen(a);
        const char *p = a, *q = b;
        size_t hits = 0;
        while (1) {
                if (*p == '\0') goto FINISHED;
                while (1) {
                        if(*q == '\0') goto FINISHED;
                        if (*p == *q) {
                                hits++;
                                break;
                        }
                        q++;
                }
                p++;
        }
FINISHED:
        return na > 0 ? (double)hits/(double)na : 0;
}

void wk_cli_parse(char **raw_args, int raw_argc, WKArg *args, size_t argc) {
        if (!raw_args || !args) wk_err("invalid arguments");
        /* 将 raw_args 转换为列表 */
        WKList *arg_list = wk_list(char *);
        for (int i = 1; i < raw_argc; i++) { /* 忽略程序名 */
                wk_list_suffix(arg_list, raw_args[i], char *);
        }
        /* 从 arg_list 中消耗长选项 */
        WKLink *it = arg_list->head;
        while (it) {
                WKLink *next = it->next;
                const char *a = wk_link_get(it, char *);
                if (strstr(a, "--") == a) { /* a 为长选项 */
                        size_t hits = 0;
                        size_t target = 0;
                        const char *opt_name;
                        for (size_t i = 0; i < argc; i++) {
                                if (!args[i].name) continue;
                                opt_name = a + 2;
                                if (strstr(opt_name, args[i].name) != opt_name) continue;
                                hits++;
                                target = i;
                                break;
                        }
                        if (hits == 0) {
                                fprintf(stderr, "invalid option --%s\n", opt_name);
                                goto NEXT_1;
                        }
                        if (hits > 1) {
                                fprintf(stderr, "repeated option!\n");
                                goto NEXT_1;
                        }
                        /* 长选项匹配成功，将 it 所指节点消耗掉 */
                        const char *type = args[target].type;
                        wk_list_del(arg_list, it);
                        if (strcmp(type, "toggle") == 0) {
                                args[target].value.toggle = true;
                                goto NEXT_1;
                        }
                        if (strcmp(type, "text") == 0) {
                                it = next;
                                next = it->next;
                                args[target].value.text = wk_link_get(it, char *);
                                wk_list_del(arg_list, it);
                                goto NEXT_1;
                        }
                        if (strcmp(type, "numeric") == 0) {
                                it = next;
                                next = it->next;
                                args[target].value.numeric = atof(wk_link_get(it, char *));
                                wk_list_del(arg_list, it);
                                goto NEXT_1;
                        }
                }
        NEXT_1:
                it = next;
        }
        /* 从 arg_list 中消耗短选项 */
        it = arg_list->head;
        while (it) {
                WKLink *next = it->next;
                const char *a = wk_link_get(it, char *);
                if (strstr(a, "-") == a) {
                        size_t hits = 0;
                        size_t target = 0;
                        const char *opt_name = a + 1;
                        for (size_t i = 0; i < argc; i++) { /* 短选项匹配 */
                                if (!args[i].name) continue;
                                double likeness = opt_match(opt_name, args[i].name);
                                if (likeness < 1.0) continue;
                                hits++;
                                target = i;
                        }
                        if (hits != 1) {
                                wk_list_del(arg_list, it);
                                fprintf(stderr, "invalid option!\n");
                                goto NEXT_2;
                        }
                        /* 短选项匹配成功，将 it 所指节点消耗掉 */
                        const char *type = args[target].type;
                        wk_list_del(arg_list, it);
                        if (strcmp(type, "toggle") == 0) {
                                args[target].value.toggle = true;
                                goto NEXT_2;
                        }
                        if (strcmp(type, "text") == 0) {
                                it = next;
                                next = it->next;
                                args[target].value.text = wk_link_get(it, char *);
                                wk_list_del(arg_list, it);
                                goto NEXT_2;
                        }
                        if (strcmp(type, "numeric") == 0) {
                                it = next;
                                next = it->next;
                                args[target].value.numeric = atof(wk_link_get(it, char *));
                                wk_list_del(arg_list, it);
                                goto NEXT_2;
                        }
                }
        NEXT_2:
                it = next;
        }
        /* 从 arg_list 里消耗无选项参数。无选项参数的顺序须与 args 中的顺序一致 */
        size_t trace = 0;
        it = arg_list->head;
        while (it) {
                WKLink *next = it->next;
                const char *a = wk_link_get(it, char *);
                while (args[trace].name) {
                        if (trace >= argc) {
                                fprintf(stderr, "invalid argument!\n");
                                goto NEXT_3;
                        }
                        trace++;
                }
                const char *type = args[trace].type;
                if (strcmp(type, "toggle") == 0) {
                        if (strcmp(a, "true") == 0) args[trace].value.toggle = true;
                        else if (strcmp(a, "false") == 0) args[trace].value.toggle = false;
                        else fprintf(stderr, "wrong toggle value!\n");
                        goto NEXT_3;
                }
                if (strcmp(type, "text") == 0) {
                        args[trace].value.text = a;
                        goto NEXT_3;
                }
                if (strcmp(type, "numeric") == 0) {
                        args[trace].value.numeric = atof(a);
                        goto NEXT_3;
                }
        NEXT_3:
                wk_list_del(arg_list, it);
                trace++;
                it = next;
        }
        wk_list_free(arg_list);
        wk_fallback;
}

void wk_arg_print(WKArg *arg) {
        if (arg->name) {
                if (strcmp(arg->type, "toggle") == 0) {
                        if (arg->value.toggle) {
                                printf("%s:\n  type: %s\n  value: true\n", arg->name, arg->type);
                        } else {
                                printf("%s:\n  type: %s\n  value: false\n", arg->name, arg->type);
                        }
                        return;
                }
                if (strcmp(arg->type, "text") == 0) {
                        printf("%s:\n  type: %s\n  value: \"%s\"\n", arg->name, arg->type, arg->value.text);
                        return;
                }
                if (strcmp(arg->type, "numeric") == 0) {
                        printf("%s:\n  type: %s\n  value: %lf\n", arg->name, arg->type, arg->value.numeric);
                        return;
                }
        } else {
                if (strcmp(arg->type, "toggle") == 0) {
                        if (arg->value.toggle) {
                                printf("argument:\n  type: %s\n  value: true\n", arg->type);
                        } else {
                                printf("argument:\n  type: %s\n  value: false\n", arg->type);
                        }
                        return;
                }
                if (strcmp(arg->type, "text") == 0) {
                        printf("argument:\n  type: %s\n  value: \"%s\"\n", arg->type, arg->value.text);
                        return;
                }
                if (strcmp(arg->type, "numeric") == 0) {
                        printf("argument:\n  type: %s\n  value: %lf\n", arg->type, arg->value.numeric);
                        return;
                }
        }
}
