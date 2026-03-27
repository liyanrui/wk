#ifndef WK_CLI_H
#define WK_CLI_H
#include <string.h>
#include <stdbool.h>
#include <wk-err.h>
#include <wk-list.h>

typedef struct {
        const char *name;  /* 选项 */
        const char *type; /* 参数类型 */
        union { /* 参数 */
                bool toggle;
                const char *text;
                double numeric;
        } value;
} WKArg;

#define WK_ARG_TOGGLE(opt) \
        (WKArg){.name = opt, .type = "toggle", .value.toggle = false}
#define WK_ARG_TEXT(opt) \
        (WKArg){.name = opt, .type = "text", .value.text = ""}
#define WK_ARG_NUMERIC(opt) \
        (WKArg){.name = opt, .type = "numeric", .value.numeric = 0}

void wk_cli_parse(char **raw_args, int raw_argc, WKArg *args, size_t argc);
void wk_arg_print(WKArg *arg);
#endif
