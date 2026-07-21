#include <string.h>
#include "wk-tree.h"

WKTree *WK_TREE(size_t u) {
        WKTree *tree = malloc(sizeof(WKTree));
        if (!tree) wk_err("failed to create WKTree object");
        tree->u = u;
        tree->root = NULL;
        return tree;
        wk_fallback_with(NULL);
}

void wk_tree_free(WKTree *tree) {
        wk_branch_free(tree->root);
        free(tree);
}

WKBranch *WK_BRANCH(WKTree *tree, void *data) {
	size_t u = tree->u;
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

WKBranch *WK_TREE_ADD(WKTree *tree, WKBranch *target, void *data) {
        WKBranch *new_branch = WK_BRANCH(tree, data);
	if (!target) {
		if (tree->root) wk_err("invalid WKTree object");
		tree->root = new_branch;
		return new_branch;
	}
	if (!target->lower) target->lower = wk_array(WKBranch *);
	wk_array_ins(target->lower, target->lower->n, new_branch, WKBranch *);
	new_branch->upper = target;
	return new_branch;
	wk_fallback_with(NULL);
}

WKBranch *WK_TREE_INS(WKTree *tree, WKBranch *target, size_t i, void *data) {
	if (!target || !target->lower) wk_err("invalid target branch");
        WKBranch *new_branch = WK_BRANCH(tree, data);
	wk_array_ins(target->lower, i, new_branch, WKBranch *);
	new_branch->upper = target;
	return new_branch;
	wk_fallback_with(NULL);
}

WKBranch *wk_tree_cut(WKTree *tree, WKBranch *branch) {
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
}
