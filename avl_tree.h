/*
 *   Copyright (c) 2011 Higepon(Taro Minowa) <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <string>
#include <algorithm>

template <class KeyType, class ValueType> class AVLTree {
 public:
  AVLTree() : root_(NULL) {
  }

  virtual ~AVLTree() {
  }

  void Add(const KeyType key, ValueType value) {
    if (root_ == NULL) {
      root_ = new Node(key, value);
      assert(CheckSanity());
      return;
    }

    for (Node* current = root_;;) {
      if (current->key == key) {
        current->value = value;
        assert(CheckSanity());
        return;
      } else if (current->key < key) {
        if (current->right == NULL) {
          Node* node_to_add = new Node(key, value, current);
          current->right = node_to_add;
          Balance(node_to_add);
          assert(CheckSanity());
          return;
        } else {
          current = current->right;
        }
      } else {
        if (current->left == NULL) {
          Node* node_to_add = new Node(key, value, current);
          current->left = node_to_add;
          Balance(node_to_add);
          assert(CheckSanity());
          return;
        } else {
          current = current->left;
        }
      }
    }
  }

  ValueType GetLowerNearest(const KeyType key) const {
  }

  ValueType Get(const KeyType key, bool* found = NULL) const {
  }

  void Remove(const KeyType key, bool* removed = NULL) {
  }

  int BalanceFactor() const {
    if (root_ == NULL) {
      return 0;
    }
    return Height(root_->left) - Height(root_->right);
  }

  bool IsBalanced() const {
    int factor = BalanceFactor();
    return factor == 0 || factor == -1 || factor == 1;
  }

  bool CheckSanity() const {
    return true;
// #ifdef MONA
//     return true;
// #else
//     std::vector<T> keys;
//     GetInOrder(root_, &keys);
//     std::vector<T> sorted = keys;
//     sort(keys.begin(), keys.end());
//     return keys == sorted;
// #endif
  }
  enum Label {
    kL = -1,
    kE = 0,
    kR = 1
  };

  struct Node {
    Node(const KeyType key, ValueType value, Node* parent = NULL,
         Node* left = NULL, Node* right = NULL, Label label = kE) :
        key(key),
        value(value),
        parent(parent),
        left(left),
        right(right),
        label(label)
    {}

    bool IsRightChild() const {
      assert(parent);
      return parent->right == this;
    }

    KeyType key;
    ValueType value;
    Node* parent;
    Node* left;
    Node* right;
    int label;
  };

  Node* Root() const {
    return root_;
  }

  // http://en.wikipedia.org/wiki/Tree_rotation

  void LeftRotation(Node* n) {
    Node* old_root = n->parent;
    Node* new_root = n;
    old_root->right = new_root->left;
    new_root->left = old_root;
    SwapRoot(old_root, new_root);
    old_root->parent = new_root;

    old_root->label -= (1 + std::max((int)new_root->label, 0));
    new_root->label -= (1 - std::min((int)old_root->label, 0));
  }

  void RotateLeft(Node*& root) {
    Node* old_root = root;
    root = root->right;
    old_root->right = root->left;
    root->left = old_root;
    // Node* new_root = n;
    old_root->label = kE;
    root->label = kE;
    old_root->parent = root; // todo
    //    old_root->right = new_root->left;
    //    new_root->left = old_root;
    //    SwapRoot(old_root, new_root);
    //    old_root->parent = new_root;
  }


  void RightRotation(Node* n) {
    Node* old_root = n->parent;
    Node* new_root = n;
    // old_root->label = kE;
    // new_root->label = kE;
    old_root->left = new_root->right;
    new_root->right = old_root;
    SwapRoot(old_root, new_root);
    old_root->parent = new_root;
    old_root->label += (1 - std::min((int)new_root->label, 0));
    new_root->label += (1 - std::max((int)old_root->label, 0));

  }


  void DoubleRightRotation(Node* n) {
    Node* old_root = n->parent;
    Node* new_root = n->left;
    // if (new_root->label == kL) {
    //   old_root->label = kE;
    //   n->label = kR;
    // } else if (new_root->label == kR) {
    //   old_root->label = kL;
    //   n->label = kE;
    // } else {
    //   old_root->label = kE;
    //   n->label = kE;
    // }
    //    new_root->label = kE;
    Node* save_n_left_left = n->left->left;
    n->left = new_root->right;
    if (new_root->right) {
      new_root->right->parent = n;
    }
    n->parent = new_root;
    new_root->right = n;
    new_root->left = old_root;
    old_root->right = save_n_left_left;
    if (save_n_left_left != NULL) {
      save_n_left_left->parent = old_root;
    }
    SwapRoot(old_root, new_root);
    old_root->parent = new_root;
    new_root->left->label  = -std::max(new_root->label, 0);
    new_root->right->label = -std::min(new_root->label, 0);
    new_root->label = kE;
  }

  void DoubleLeftRotation(Node* n) {
    Node* old_root = n->parent;
    Node* new_root = n->right;
    // if (new_root->label == kL) {
    //   old_root->label = kR;
    //   n->label = kE;
    // } else if (new_root->label == kR) {
    //   old_root->label = kE;
    //   n->label = kL;
    // } else {
    //   old_root->label = kE;
    //   n->label = kE;
    // }
    // new_root->label = kE;
    Node* save_n_right_right = n->right->right;
    n->right = new_root->left;
    if (new_root->left) {
      new_root->left->parent = n;
    }
    n->parent = new_root;
    new_root->left = n;
    new_root->right = old_root;
    old_root->left = save_n_right_right;
    if (save_n_right_right != NULL) {
      save_n_right_right->parent = old_root;
    }
    SwapRoot(old_root, new_root);
    old_root->parent = new_root;

    new_root->left->label  = -std::max(new_root->label, 0);
    new_root->right->label = -std::min(new_root->label, 0);
    new_root->label = kE;
  }

  void Balance(Node* n) {
    assert(n);
    if (n->parent == NULL) {
      return;
    }

    Node* parent = n->parent;
    if (n->IsRightChild()) {
      if (parent->label == kE) {
        parent->label = kR;
        Balance(parent);
      } else if (parent->label == kR) {
        if (n->label == kR) {
          n->parent->label++;
          LeftRotation(n);
          //          RotateLeft(n->parent);
        } else if (n->label == kL) {
          DoubleRightRotation(n);
        } else {
          assert(0);
        }
      } else {
        parent->label = kE;
        return;
      }
    } else {
      if (parent->label == kE) {
        parent->label = kL;
        Balance(parent);
      } else if (parent->label == kL) {
        if (n->label == kL) {
          n->parent->label--;
          RightRotation(n);
        } else if (n->label == kR) {
          DoubleLeftRotation(n);
        } else {
          assert(0);
        }
      } else {
        parent->label = kE;
        return;
      }
    }
  }


 private:

  void SwapRoot(Node* old_root, Node* new_root) {
    if (old_root->parent == NULL) {
      root_ = new_root;
    } else {
      if (old_root->IsRightChild()) {
        old_root->parent->right = new_root;
      } else {
        old_root->parent->left = new_root;
      }
    }
    new_root->parent = old_root->parent;
  }

  int Height(Node* n) const {
    if (n == NULL) {
      return 0;
    }
    int l = Height(n->left);
    int r = Height(n->right);
    return std::max(l, r) + 1;
  }

  Node* root_;
};

#endif  // AVL_TREE_H_
