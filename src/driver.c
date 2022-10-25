#include "rbtree.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();
    node_t *p = rbtree_insert(t, 5);
    p = rbtree_insert(t, 7);
    p = rbtree_insert(t, 1);
    p = rbtree_insert(t, 8);
    p = rbtree_insert(t, 3);
    p = rbtree_insert(t, 6);
    p = rbtree_insert(t, 4);
    p = rbtree_insert(t, 2);
    printf("%d\n", p->key);
    printf("%d\n", p->left->key);
    printf("%d\n", p->right->key);
}