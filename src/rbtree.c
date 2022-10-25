#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t* newNode = (node_t*)calloc(1, sizeof(node_t)); // nil 노드 생성
  newNode->color = RBTREE_BLACK;
  p->root = p->nil = newNode;

  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void left_rotate(rbtree*t, node_t* standard){
  node_t* temp = standard->right; //회전의 기준이 되는 노드(standard)의 오른쪽 자식을 temp로 설정한다.
  standard->right = temp->left; // temp의 왼쪽 자식을 standard의 오른쪽 자식으로 만들어 준다.
  
  if(temp->left != t->nil) { // temp의 왼쪽 자식이 nil노드가 아닐때
    temp->left->parent = standard; // temp의 왼쪽 자식의 부모를 standard로 만들어 준다.
  }
  temp->parent = standard->parent; // temp의 부모를 standard의 부모로 바꾸어 준다.
  
  if(standard->parent == t->nil) { // standard가 루트 노드일 때
    t->root = temp; // root를 temp로 바꾸어 준다.
  }
  else if(standard == standard->parent->left) { // standard가 부모의 왼쪽에 있을 때
    standard->parent->left = temp; // standard 부모의 왼쪽에 temp를 연결해준다.
  }
  else { // standard가 부모의 오른쪽에 있을 때
    standard->parent->right = temp; // standard 부모의 오른쪽에 temp를 연결해준다.
  }
  temp->left = standard; // temp의 왼쪽에 standard를 연결해준다.
  standard->parent = temp; // standard의 부모를 temp로 설정한다.
}

void right_rotate(rbtree* t, node_t* standard) {
  node_t* temp = standard->left;
  standard->left = temp->right;

  if(temp->right != t->nil) {
    temp->right->parent = standard;
  }
  temp->parent = standard->parent;

  if (standard->parent == t->nil) {
    t->root = temp;
  }
  else if (standard == standard->parent->left) {
    standard->parent->left = temp;
  }
  else {
    standard->parent->right = temp;
  }
  temp->right = standard;
  standard->parent = temp;
}

void rbtree_insert_fixup(rbtree* t, node_t* targetNode){
  node_t* temp = (node_t*)calloc(1, sizeof(node_t));

  while (targetNode->parent->color == RBTREE_RED) {
    if (targetNode->parent == targetNode->parent->parent->left) { // 부모가 할아버지의 왼쪽 자녀일 때
      temp = targetNode->parent->parent->right; // temp를 삼촌 노드로 설정

      if (temp->color == RBTREE_RED) { // case1. 삼촌의 색이 red일 때
        targetNode->parent->color = RBTREE_BLACK;
        temp->color= RBTREE_BLACK;
        targetNode->parent->parent->color = RBTREE_RED;
        targetNode = targetNode->parent->parent;
      }

      else { // 삼촌의 색이 black일 때
        if (targetNode == targetNode->parent->right) { // case2. 새로운 노드가 부모의 오른쪽* 자녀일 때
          targetNode = targetNode->parent;
          left_rotate(t, targetNode);
        }
        else {
          targetNode->parent->color = RBTREE_BLACK; //case3. 새로운 노드가 부모의 왼쪽* 자녀일 때
          targetNode->parent->parent->color = RBTREE_RED;
          right_rotate(t, targetNode->parent->parent);
        }
      }
    }

    else {
        temp = targetNode->parent->parent->left;

        if (temp->color == RBTREE_RED) {
          targetNode->parent->color = RBTREE_BLACK;
          temp->color= RBTREE_BLACK;
          targetNode->parent->parent->color = RBTREE_RED;
          targetNode = targetNode->parent->parent;
        }

        else {
          if (targetNode == targetNode->parent->left) {
            targetNode = targetNode->parent;
            right_rotate(t, targetNode);
          }
          else {
            targetNode->parent->color = RBTREE_BLACK;
            targetNode->parent->parent->color = RBTREE_RED;
            left_rotate(t, targetNode->parent->parent);
          }
        }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* tNode = t->nil;
  node_t* rNode = t->root;
  node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

  while (rNode != t->nil) {
    tNode = rNode;
    if (rNode->key > key) {
      rNode = rNode->left;
    }
    else {
      rNode = rNode->right;
    }
  }
  newNode->parent = tNode;
  newNode->key = key;

  if (tNode == t->nil) {
    t->root = newNode;
  }
  else if(newNode->key < tNode->key) {
    tNode->left = newNode;
  }
  else {
    tNode->right = newNode;
  }
  newNode->left = newNode->right = t->nil;
  newNode->color = RBTREE_RED;
  rbtree_insert_fixup(t, newNode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* rNode = t->root;
  while(rNode != t->nil) {
    if (rNode->key == key) {
      return rNode;
    }
    else if (rNode->key > key) {
      rNode = rNode->left;
    }
    else {
      rNode = rNode->right;
    }
  }
  if (rNode == t->nil) {
    return NULL;
  }
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* rNode = t->root;
  while(rNode != t->nil) {
    rNode = rNode->left;
  }
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* rNode = t->root;
  while(rNode != t->nil) {
    rNode = rNode->right;
  }
  return t->root;
}

node_t* node_min(rbtree* t, node_t* root) {
  while(root->left != t->nil) {
    root = root->left;
  }
  return root;
}

void rb_transplant(rbtree* t, node_t* del, node_t* replace) {
  if (del->parent == t->nil) {
    t->root = replace;
  }
  else if (del == del->parent->left) {
    del->parent->left = replace;
  }
  else {
    del->parent->right = replace;
  }
  replace->parent = del->parent;
}

void delete_fixup(rbtree*t, node_t* x) {
  node_t* w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RBTREE_RED) { // case1. doubly black의 형제가 red일 때
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) { // case2. doubly black의 형제가 black이고 그 형제의 두 자녀 모두 black일 때
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if ((w->left->color == RBTREE_RED) && (w->right->color == RBTREE_BLACK)) { // case3. doubly black의 형제가 black이고 그 형제의 왼쪽 자녀가 red, 오른쪽 자녀는 black일 때
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        else { // case4. double black의 오른쪽 형제가 black이고 그 형제의 오른쪽 자녀가 red일 때 
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          left_rotate(t, x->parent);
          x = t->root;
        }
      }
    }

    else {
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->right;
      }
      if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if ((w->right->color == RBTREE_RED) && (w->left->color == RBTREE_BLACK)) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->right;
        }
        else {
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->left->color = RBTREE_BLACK;
          right_rotate(t, x->parent);
          x = t->root;
        }
      }
      
      
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t* tNode = p;
  node_t* bNode;

  color_t tNode_original_color = tNode->color;

  if (p->left == t->nil) {
    bNode = p->right;
    rb_transplant(t, p, p->left);
  }
  else if (p->right == t->nil) {
    bNode = p->left;
    rb_transplant(t, p, p->left);
  }
  else {
    tNode = node_min(t, p->right);
    tNode_original_color = tNode->color;
    bNode = tNode->right;

    if (tNode->parent == p) {
      bNode->parent = tNode;
    }
    else {
      rb_transplant(t, tNode, tNode->right);
      tNode->right = p->right;
      tNode->right->parent = tNode;
    }
    rb_transplant(t, p, tNode);
    tNode->left = p->left;
    tNode->left->parent = tNode;
    tNode->color = p->color;
  }
  if (tNode_original_color == RBTREE_BLACK) {
    delete_fixup(t, bNode);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
