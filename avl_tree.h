/*
 *   Copyright (c) 2011 Higepon(Taro Minowa) <higepon@users.sourceforge.jp>
 *                      Brad Appleton <bradapp@enteract.com>
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

// Based on AvlTrees by Brad Appleton <bradapp@enteract.com>.
// http://www.cmcrossroads.com/bradapp/ftp/src/libs/C++/AvlTrees.html
// See LICENSE_AvlTrees.txt

#include <stdlib.h>
#include <assert.h>

template <class KeyType, class ValueType> class AVLTree {
 private:
  enum CompareResult {
    kMinCmp = -1,
    kEqCmp = 0,
    kMaxCmp = 1
  };

  enum Direction {
    kLeft = 0,
    kRight = 1
  };

  class Comparable {
   private:
    KeyType key;
    ValueType value;

   public:
    Comparable(KeyType key, ValueType value) : key(key), value(value) {}

    CompareResult Compare(KeyType key) const {
      return (key == this->key) ? kEqCmp
          : ((key < this->key) ? kMinCmp : kMaxCmp);
    }

    KeyType Key() const {
      return  key;
    }

    ValueType Value() const {
      return value;
    }

    void SetValue(ValueType v) {
      value = v;
    }
  };

 public:

  enum BalanceFactor {
    kE = 0,
    kL = -1,
    kR = 1
  };

  struct Node {
    explicit Node(Comparable* item) :
        item(item),
        balance_factor(kE) {
      children[kLeft] = NULL;
      children[kRight] = NULL;
    }

    ~Node() {
      if (Left()) {
        delete Left();
      }
      if (Right()) {
        delete Right();
      }
    }

    bool IsLeftImbalance() const {
      return balance_factor < kL;
    }

    bool IsRightImbalance() const {
      return balance_factor > kR;
    }

    Node* Right() const {
      return children[kRight];
    }

    Node* Left() const {
      return children[kLeft];
    }

    static Direction Opposite(Direction dir) {
      return static_cast<Direction>(1 - static_cast<int>(dir));
    }

    static int RotateOnce(Node*& root, Direction dir) {
      Direction other_dir = Opposite(dir);
      Node* oldRoot = root;

      // See if otherDir subtree is balanced. If it is, then this
      // rotation will *not* change the overall tree height.
      // Otherwise, this rotation will shorten the tree height.
      int  height_change = (root->children[other_dir]->balance_factor == 0)
          ? kHeightNoChange : kHeightChange;

      // assign new root
      root = oldRoot->children[other_dir];

      // new-root exchanges it's "dir" mySubtree for it's parent
      oldRoot->children[other_dir] = root->children[dir];
      root->children[dir] = oldRoot;

      // update balances
      oldRoot->balance_factor =
          -((dir == kLeft) ?
            --(root->balance_factor) : ++(root->balance_factor));

      return  height_change;
    }

    static int RotateTwice(Node*& root, Direction dir) {
      Direction other_dir = Opposite(dir);
      Node* old_root = root;
      Node* old_other_dir_subtree = root->children[other_dir];

      // assign new root
      root = old_root->children[other_dir]->children[dir];

      // new-root exchanges it's "dir" mySubtree for it's grandparent
      old_root->children[other_dir] = root->children[dir];
      root->children[dir] = old_root;

      // new-root exchanges it's "other-dir" mySubtree for it's parent
      old_other_dir_subtree->children[dir] = root->children[other_dir];
      root->children[other_dir] = old_other_dir_subtree;

      // update balances
      root->Left()->balance_factor =
          -std::max(static_cast<int>(root->balance_factor), 0);
      root->Right()->balance_factor =
          -std::min(static_cast<int>(root->balance_factor), 0);
      root->balance_factor = 0;

      // A double rotation always shortens the overall height of the tree
      return kHeightChange;
    }

    static Comparable* Insert(Comparable* item, Node*& root) {
      int change;
      return Insert(item, root, change);
    }

    static Comparable* Insert(Comparable* item, Node*& root, int& change) { // NOLINT
      if (root == NULL) {
        root = new Node(item);
        change = kHeightChange;
        return NULL;
      }

      Comparable* found = NULL;
      CompareResult result = root->Compare(item->Key());
      Direction dir = (result == kMinCmp) ? kLeft : kRight;

      int increase = 0;
      if (result != kEqCmp) {
        found = Insert(item, root->children[dir], change);
        if (found) {
          return found;
        }
        increase = result * change;
      } else  {
        increase = kHeightNoChange;
        return root->item;
      }

      root->balance_factor += increase;
      change = (increase && root->balance_factor)
          ? (1 - ReBalance(root)) : kHeightNoChange;
      return NULL;
    }

    static int ReBalance(Node*& root) {
      int height_change = kHeightNoChange;

      if (root->IsLeftImbalance()) {
        // Need a right rotation
        if (root->Left()->balance_factor == kR) {
          // RL rotation needed
          height_change = RotateTwice(root, kRight);
        } else {
          // RR rotation needed
          height_change = RotateOnce(root, kRight);
        }
      } else if (root->IsRightImbalance()) {
        // Need a left rotation
        if (root->Right()->balance_factor == kL) {
          // LR rotation needed
          height_change = RotateTwice(root, kLeft);
        } else {
          // LL rotation needed
          height_change = RotateOnce(root, kLeft);
        }
      }

      return height_change;
    }

    static Comparable* Remove(KeyType key, Node*& root, CompareResult cmp) {
      int change;
      return Remove(key, root, change, cmp);
    }

    static Comparable* Remove(const KeyType key,
                              Node*& root,
                              int& change,
                              CompareResult cmp) {
      if (root == NULL) {
        // Key not found
        change = kHeightNoChange;
        return NULL;
      }

      Comparable* found = NULL;
      int decrease = 0;

      CompareResult result = root->Compare(key, cmp);
      Direction dir = (result == kMinCmp) ? kLeft : kRight;

      if (result != kEqCmp) {
        found = Remove(key, root->children[dir], change, cmp);
        if (!found) {
          return found;
        }
        decrease = result * change;
      } else  {
        found = root->item;
        if ((root->Left() == NULL) &&
            (root->Right() == NULL)) {
          delete root;
          root = NULL;
          change = kHeightChange;
          return  found;
        } else if ((root->Left() == NULL) ||
                   (root->Right() == NULL)) {
          Node* toDelete = root;
          root = root->children[(root->Right()) ? kRight : kLeft];
          change = kHeightChange;
          toDelete->children[kLeft] = toDelete->children[kRight] = NULL;
          delete  toDelete;
          return  found;
        } else {
          root->item = Remove(key, root->children[kRight],
                              decrease, kMinCmp);
        }
      }
      root->balance_factor -= decrease;

      if (decrease) {
        if (root->balance_factor) {
          change = ReBalance(root);
        } else {
          change = kHeightChange;
        }
      } else {
        change = kHeightNoChange;
      }
      return  found;
    }


    CompareResult Compare(KeyType key, CompareResult cmp = kEqCmp) const {
      switch (cmp) {
        case kEqCmp:
          return item->Compare(key);
        case kMinCmp:
          return  (children[kLeft] == NULL) ? kEqCmp : kMinCmp;
        default:
          assert(cmp == kMaxCmp);
          return (children[kRight] == NULL) ? kEqCmp : kMaxCmp;
      }
    }


    Node* children[2];
    Comparable* item;
    int8_t balance_factor;

   private:
    Node() {}
    Node(const Node& n) {}
    Node & operator=(const Node&) {}
  };

  AVLTree() : root_(NULL) {
  }

  virtual ~AVLTree() {
    if (root_) {
      delete root_;
    }
  }

  Node* Root() const {
    return root_;
  }

  void Add(const KeyType key, const ValueType value) {
    Comparable* item = new Comparable(key, value);
    Comparable* result = Node::Insert(item, root_);
    if (result) {
      result->SetValue(value);
      delete item;
    }
  }

  Comparable* Remove(const KeyType key, CompareResult cmp = kEqCmp) {
    return Node::Remove(key, root_, cmp);
  }

  bool IsBalanced() const {
    if (root_ == NULL) {
      return true;
    }
    int diff = abs(Height(root_->Left()) - Height(root_->Right()));
    return diff <= 1;
  }

  bool IsEmpty() const {
    return root_ == NULL;
  }

 private:
  enum HeightEffect {
    kHeightNoChange = 0,
    kHeightChange = 1
  };

  int Height(Node* n) const {
    if (n == NULL) {
      return 0;
    }
    int l = Height(n->Left());
    int r = Height(n->Right());
    return std::max(l, r) + 1;
  }

  Node* root_;
};

#endif  // AVL_TREE_H_
