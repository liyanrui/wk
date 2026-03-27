#include "wk-err.h"
#include "wk-list.h"

int main(void) {
        printf(">>> testing list!\n");
        /* 构造链表 */
        WKList *list = wk_list(int);
        for (int i = 0; i < 5; i++) {
                wk_list_suffix(list, i, int);
        }
        /* 删除第 2 个节点 */
        wk_list_del(list, list->head);
        wk_list_del(list, list->head->next);
        wk_list_del(list, list->tail);
        /* 遍历 list */
        bool success = true;
        int arr[] = {1, 3};
        size_t i = 0;
        for (WKLink *it = list->head; it; it = it->next) {
                if (arr[i] != wk_list_get(list, it, int)) {
                        success = false;
                        break;
                }
                i++;
        }
        WK_HOPE(success);
        wk_list_free(list);

        printf("OK!\n");
        return 0;
}
