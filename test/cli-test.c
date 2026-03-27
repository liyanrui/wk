#include "wk-cli.h"

int main(int argc, char **argv) {
        printf(">>> testing cli...\n");
        /* 准备选项/参数表 */
        WKArg args[] = {
                WK_ARG_TOGGLE("foo"),
                WK_ARG_TEXT("bar"),
                WK_ARG_NUMERIC("num"),
                WK_ARG_TEXT(NULL)
        };
        size_t n = sizeof(args) / sizeof(WKArg);
        /* 解析 argv */
        wk_cli_parse(argv, argc, args, n);
        WK_HOPE(args[0].value.toggle);
        WK_HOPE(strcmp(args[1].value.text, "hello world") == 0);
        WK_HOPE(args[2].value.numeric == 3.0);
        WK_HOPE(strcmp(args[3].value.text, "i am free!") == 0);
        printf("OK!\n");
        return 0;
}
