#include "wk-tree.h"

/************************/
/* - 42                 */
/*   |                  */
/*   |--- 0             */
/*   |--- 1             */
/*   |--- 2 ----.       */
/*   |--- 3     |       */
/*              |--- 4  */
/*              |--- 6  */
/************************/
int main(void) {
        printf(">>> testing tree ...\n");
        WKTree *tree = wk_tree(int);
        /* 根 */
        wk_tree_add(tree, NULL, 42, int);
        /* 第一层 */
        wk_tree_add(tree, tree->root, 0, int);
        wk_tree_add(tree, tree->root, 1, int);
        WKBranch *b2 = wk_tree_add(tree, tree->root, 2, int);
        wk_tree_add(tree, tree->root, 3, int);
        /* 第二层 */
        wk_tree_add(tree, b2, 4, int);
        wk_tree_add(tree, b2, 6, int);
        /* 测试根的第 4 个子分支的值是否为 3 */
        WKBranch *b3 = wk_array_get(tree->root->lower, 3, WKBranch *);
        WK_HOPE(3 == wk_branch_get(b3, int));
        /* 测试 b2 的第 2 个子分支的值是否为 6 */
        WKBranch *b2_1 = wk_array_get(b2->lower, 1, WKBranch *);
        WK_HOPE(6 == wk_branch_get(b2_1, int));
        /* 将 b2 从树中切除，并单独释放 */
        wk_tree_cut(tree, b2);
        wk_branch_free(b2);
        /* 释放树的剩余分支 */
        WK_HOPE(3 == tree->root->lower->n);
        wk_tree_free(tree);
        printf("OK!\n");
        return 0;
}
