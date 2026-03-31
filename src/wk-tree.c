#include <string.h>
#include "wk-tree.h"

WKTree *WK_TREE(size_t u) {
        if (u == 0) wk_err("invalid size");
        WKTree *tree = malloc(sizeof(WKTree));
        if (!tree) wk_err("failed to create WKTree object");
        tree->u = u;
        tree->root = NULL;
        return tree;
        wk_fallback_with(NULL);
}

void wk_tree_free(WKTree *tree) {
        if (!tree) wk_err("invalid WKTree object");
        wk_branch_free(tree->root);
        free(tree);
        wk_fallback;
}

WKBranch *WK_BRANCH(WKTree *tree, void *data, size_t u) {
        if (tree->u != u) wk_err("size not matched");
        WKBranch *branch = malloc(sizeof(WKBranch));
        if (!branch) wk_err("failed to create WKBranch object");
        branch->body = malloc(u);
        if (!branch->body) wk_err("failed to create WKBranch object");
        memcpy(branch->body, data, u);
        branch->upper = NULL;
        branch->lower = NULL;
        return branch;
        wk_fallback_with(NULL);
}

void wk_branch_free(WKBranch *branch) {
        if (!branch) return;
        WKArray *lower = branch->lower;
        if (lower) {
                for (size_t i = 0; i < lower->n; i++) {
                        wk_branch_free(wk_array_get(lower, i, WKBranch *));
                }
                wk_array_free(lower);
        }
        free(branch->body);
        free(branch);
}

void wk_tree_graft(WKTree *tree, WKBranch *target, WKBranch *branch) {
        if (!tree || !branch) wk_err("invalid tree or branch");
        if (!tree->root) {
                tree->root = branch;
                return;
        }
        if (!target) wk_err("invalid target branch");
        branch->upper = target;
        if (!target->lower) target->lower = wk_array(WKBranch *);
        wk_array_add(target->lower, branch, WKBranch *);
        wk_fallback;
}

WKBranch *WK_TREE_ADD(WKTree *tree, WKBranch *target, void *data, size_t u) {
        WKBranch *new_branch = WK_BRANCH(tree, data, u);
        wk_tree_graft(tree, target, new_branch);
        return new_branch;
}

WKBranch *wk_tree_cut(WKTree *tree, WKBranch *branch) {
        if (!tree || !branch) wk_err("invalid tree or branch");
        if (tree->root == branch) return branch;
        /* 从上层节点的子节点集里删除 branch */
        WKArray *mates = branch->upper->lower;
        for (size_t i = 0; i < mates->n; i++) {
                if (branch == wk_array_get(mates, i, WKBranch *)) {
                        wk_array_del(mates, i);
                        break;
                }
        }
        branch->upper = NULL;
        return branch;
        wk_fallback_with(NULL);
}

void *WK_TREE_GET(WKTree *tree, WKBranch *target, size_t u) {
        if (!tree || !target) wk_err("invalid WKTree object");
        if (tree->u < u) wk_err("size not matched");
        return target->body;
        wk_fallback_with(NULL);
}
