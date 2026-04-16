#ifndef WK_TREE_H
#define WK_TREE_H
#include <wk-err.h>
#include <wk-array.h>

typedef struct wk_branch {
        void *body; /* 数据区域 */
        struct wk_branch *upper; /* 上层节点 */
        WKArray *lower;        /* 下层节点 */
} WKBranch;

typedef struct wk_tree {
        size_t u;   /* 分支数据区域长度（字节数） */
        WKBranch *root;
} WKTree;

WKTree *WK_TREE(size_t u);
#define wk_tree(T) WK_TREE(sizeof(T))
void wk_tree_free(WKTree *tree);

WKBranch *WK_BRANCH(WKTree *tree, void *data, size_t u);
#define wk_branch(tree, val, T) WK_BRANCH(tree, &(T){val}, sizeof(T))
void wk_branch_free(WKBranch *branch);

/* 将一个现有的分支插入到树中，作为指定分支的子分支 */
void wk_tree_graft(WKTree *tree, WKBranch *target, WKBranch *branch);

/* 基于 wk_tree_graft 实现 */
WKBranch *WK_TREE_ADD(WKTree *tree, WKBranch *target, void *data, size_t u);
#define wk_tree_add(tree, branch, val, T)        \
        WK_TREE_ADD(tree, branch, &(T){val}, sizeof(T))

/* 将分支从树中砍下来 */
WKBranch *wk_tree_cut(WKTree *tree, WKBranch *branch);

/* /\* 查询一个分支在树中的层次（根节点的层次为 0） *\/ */
/* /\* 切勿将其理解为查询树的深度！ *\/ */
/* size_t wk_tree_level(WKTree *tree); */

#define wk_branch_get(branch, T) (*(T*)(branch)->body)
#endif
