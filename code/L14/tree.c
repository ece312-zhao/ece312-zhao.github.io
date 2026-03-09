#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    double val;
    struct TreeNode *left, *right;
} TreeNode;

void print_tree_node(TreeNode *node, size_t depth, double marker) {
    if (!node) return;

    print_tree_node(node->right, depth + 1, marker);
    for (size_t i = 0; i < depth; i++) {
        printf("    ");
    }

    if (marker == node->val) {
        printf("%.1f*\n", node->val);
    } else {
        printf("%.1f\n", node->val);
    }

    print_tree_node(node->left, depth + 1, marker);
}


void print_tree(TreeNode *node, double marker) {
    puts("-----------------------------");
    print_tree_node(node, 0, marker);
    puts("-----------------------------");
}

TreeNode *build_binary_search_tree(double *arr, size_t sz) {
    if (sz == 0) return NULL;

    size_t mid = sz / 2;

    TreeNode *node = malloc(sizeof(TreeNode));
    assert(node);
    node->val = arr[mid];

    // build for the left half
    size_t left_sz = mid;
    node->left = build_binary_search_tree(arr, left_sz);

    // build for the right half
    size_t right_sz = sz - mid - 1;
    node->right = build_binary_search_tree(&arr[mid + 1], right_sz);
    return node;
}

void free_binary_tree(TreeNode *root) {
    if (!root) return;

    free_binary_tree(root->left);
    free_binary_tree(root->right);
    free(root);
}

bool search_in_bst(TreeNode *node, double target) {
    if (!node) return false;

    if (target == node->val) {
        return true;
    } else if (target < node->val) {
        return search_in_bst(node->left, target);
    } else {
        return search_in_bst(node->right, target);
    }
}

void preorder_traversal(TreeNode *node) {
    if (!node) return;

    printf("%.1f ", node->val);
    preorder_traversal(node->left);
    preorder_traversal(node->right);
}

void inorder_traversal(TreeNode *node) {
    if (!node) return;

    inorder_traversal(node->left);
    printf("%.1f ", node->val);
    inorder_traversal(node->right);
}

void postorder_traversal(TreeNode *node) {
    if (!node) return;

    postorder_traversal(node->left);
    postorder_traversal(node->right);
    printf("%.1f ", node->val);
}


typedef struct BFNode {
    TreeNode *node;
    struct BFNode *next;
} BFNode;

void bf_traversal(TreeNode *root) {
    if (!root) return;

    BFNode *head = malloc(sizeof(BFNode)), *tail = head;
    head->node = root;
    head->next = NULL;

    while (head) {
        TreeNode *node = head->node;

        if (node) {
            printf("%.1f ", node->val);

            BFNode *left = malloc(sizeof(BFNode));
            left->node = node->left;
            BFNode *right = malloc(sizeof(BFNode));
            right->node = node->right;

            left->next = right;
            right->next = NULL;

            tail->next = left;
            tail = right;
        }

        BFNode *next = head->next;
        free(head);
        head = next;
    }
}

int tree_height(TreeNode *root) {
    if (!root) return -1;

    int left_h = tree_height(root->left);
    int right_h = tree_height(root->right);

    if (left_h > right_h) {
        return left_h + 1;
    } else {
        return right_h + 1;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;

    size_t sz = strtoul(argv[1], NULL, 10);
    double *arr = calloc(sz, sizeof(*arr));
    assert(arr);
    for (size_t i = 0; i < sz; i++) {
        arr[i] = (double)i;
    }

    TreeNode *tree = build_binary_search_tree(arr, sz);
    free(arr);

    printf("Found = %d\n", search_in_bst(tree, 5.0));
    printf("Found = %d\n", search_in_bst(tree, 10.0));

    print_tree(tree, 12);

    preorder_traversal(tree); puts("");
    inorder_traversal(tree); puts("");
    postorder_traversal(tree); puts("");

    bf_traversal(tree); puts("");

    printf("Tree height: %d\n", tree_height(tree));

    free_binary_tree(tree);
    return 0;
}

