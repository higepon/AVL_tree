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
          current->right = new Node(key, value);
          assert(CheckSanity());
          return;
        } else {
          current = current->right;
        }
      } else {
        if (current->left == NULL) {
          current->left = new Node(key, value);
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

 private:
  struct Node {
    Node(const KeyType key, ValueType value,
         Node* left = NULL, Node* right = NULL) :
        key(key),
        value(value),
        left(left),
        right(right) {}

    KeyType key;
    ValueType value;
    Node* left;
    Node* right;
  };

  int Height(Node* n) const {
    if (n == NULL) {
      return 0;
    }
    return std::max(Height(n->left) + 1, Height(n->right) + 1);
  }

  Node* root_;
};

#endif  // AVL_TREE_H_
