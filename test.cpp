/*
 *   Copyright (c) 2011  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
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
#include <stdint.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include "./avl_tree.h"

namespace {

typedef AVLTree<int, int> IntAVLTree;
typedef IntAVLTree::Node Node;

static void ExpectNodeEq(int expected_key, int expected_parent_key,
                         IntAVLTree::Label expected_label, Node* node) {
  ASSERT_TRUE(node != NULL);
  EXPECT_EQ(expected_key, node->key);
  EXPECT_EQ(expected_label, node->label);
  if (expected_parent_key != 0) {
    ASSERT_TRUE(node->parent != NULL);
    EXPECT_EQ(expected_parent_key, node->parent->key);
  }
}
#define EXPECT_NODE_EQ(key, parent_key, label, node) { \
  SCOPED_TRACE(""); \
  ExpectNodeEq(key, parent_key, label, node);   \
}

class AVLTreeTest : public ::testing::Test {
 protected:
  IntAVLTree tree_;
 public:
  AVLTreeTest()  {
  }

  virtual ~AVLTreeTest() {
  }

  void MakePreDoubleRightRotationTree() {
    tree_.Add(5, 5);
    EXPECT_NODE_EQ(5, 0, IntAVLTree::kE, tree_.Root());

    tree_.Add(3, 3);
    EXPECT_NODE_EQ(5, 0, IntAVLTree::kL, tree_.Root());
    EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left);

    tree_.Add(9, 9);
    EXPECT_NODE_EQ(5, 0, IntAVLTree::kE, tree_.Root());
    EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left);
    EXPECT_NODE_EQ(9, 5, IntAVLTree::kE, tree_.Root()->right);

    tree_.Add(7, 7);
    EXPECT_NODE_EQ(5, 0, IntAVLTree::kR, tree_.Root());
    EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left);
    EXPECT_NODE_EQ(9, 5, IntAVLTree::kL, tree_.Root()->right);
    EXPECT_NODE_EQ(7, 9, IntAVLTree::kE, tree_.Root()->right->left);

    tree_.Add(10, 10);
    EXPECT_NODE_EQ(5, 0, IntAVLTree::kR, tree_.Root());
    EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left);
    EXPECT_NODE_EQ(9, 5, IntAVLTree::kE, tree_.Root()->right);
    EXPECT_NODE_EQ(7, 9, IntAVLTree::kE, tree_.Root()->right->left);
    EXPECT_NODE_EQ(10, 9, IntAVLTree::kE, tree_.Root()->right->right);
  }

  void MakePreDoubleLeftRotationTree() {
    tree_.Add(9, 9);
    tree_.Add(5, 5);
    tree_.Add(11, 11);
    tree_.Add(3, 3);
    tree_.Add(7, 7);

    EXPECT_NODE_EQ(9, 0, IntAVLTree::kL, tree_.Root());
    EXPECT_NODE_EQ(5, 9, IntAVLTree::kE, tree_.Root()->left);
    EXPECT_NODE_EQ(11, 9, IntAVLTree::kE, tree_.Root()->right);
    EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left->left);
    EXPECT_NODE_EQ(7, 5, IntAVLTree::kE, tree_.Root()->left->right);
  }
};


TEST_F(AVLTreeTest, AddOneNode) {
  tree_.Add(1, 1);
  EXPECT_NODE_EQ(1, 0, IntAVLTree::kE, tree_.Root());
}

TEST_F(AVLTreeTest, AddTwoNodesRightSide) {
  tree_.Add(3, 3);
  tree_.Add(5, 5);
  EXPECT_NODE_EQ(3, 0, IntAVLTree::kR, tree_.Root());
  EXPECT_NODE_EQ(5, 3, IntAVLTree::kE, tree_.Root()->right);
}

TEST_F(AVLTreeTest, AddThreeNodesRightSide) {
  tree_.Add(3, 3);
  tree_.Add(5, 5);
  tree_.Add(7, 7);
  EXPECT_NODE_EQ(5, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left);
  EXPECT_NODE_EQ(7, 5, IntAVLTree::kE, tree_.Root()->right);
}

TEST_F(AVLTreeTest, DoubleRightRotation1) {
  MakePreDoubleRightRotationTree();

  tree_.Add(6, 6);
  EXPECT_NODE_EQ(7, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(5, 7, IntAVLTree::kE, tree_.Root()->left);
  EXPECT_NODE_EQ(9, 7, IntAVLTree::kR, tree_.Root()->right);
  EXPECT_EQ(NULL, tree_.Root()->right->left);
  EXPECT_NODE_EQ(10, 9, IntAVLTree::kE, tree_.Root()->right->right);
  EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left->left);
  EXPECT_NODE_EQ(6, 5, IntAVLTree::kE, tree_.Root()->left->right);
}

TEST_F(AVLTreeTest, DoubleRightRotation2) {
  MakePreDoubleRightRotationTree();

  tree_.Add(8, 8);
  EXPECT_NODE_EQ(7, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(5, 7, IntAVLTree::kL, tree_.Root()->left);
  EXPECT_NODE_EQ(9, 7, IntAVLTree::kE, tree_.Root()->right);
  EXPECT_NODE_EQ(8, 9, IntAVLTree::kE, tree_.Root()->right->left);
  EXPECT_NODE_EQ(10, 9, IntAVLTree::kE, tree_.Root()->right->right);
  EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left->left);
  EXPECT_EQ(NULL, tree_.Root()->left->right);
}

TEST_F(AVLTreeTest, DoubleLeftRotation1) {
  MakePreDoubleLeftRotationTree();

  tree_.Add(6, 6);
  EXPECT_NODE_EQ(7, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(5, 7, IntAVLTree::kE, tree_.Root()->left);
  EXPECT_NODE_EQ(9, 7, IntAVLTree::kR, tree_.Root()->right);
  EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left->left);
  EXPECT_NODE_EQ(6, 5, IntAVLTree::kE, tree_.Root()->left->right);
  EXPECT_EQ(NULL, tree_.Root()->right->left);
  EXPECT_NODE_EQ(11, 9, IntAVLTree::kE, tree_.Root()->right->right);
}

TEST_F(AVLTreeTest, DoubleLeftRotation2) {
  MakePreDoubleLeftRotationTree();

  tree_.Add(8, 8);
  EXPECT_NODE_EQ(7, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(5, 7, IntAVLTree::kL, tree_.Root()->left);
  EXPECT_NODE_EQ(9, 7, IntAVLTree::kE, tree_.Root()->right);
  EXPECT_NODE_EQ(3, 5, IntAVLTree::kE, tree_.Root()->left->left);
  EXPECT_EQ(NULL, tree_.Root()->left->right);
  EXPECT_NODE_EQ(11, 9, IntAVLTree::kE, tree_.Root()->right->right);
  EXPECT_NODE_EQ(8, 9, IntAVLTree::kE, tree_.Root()->right->left);
}

TEST_F(AVLTreeTest, RightRotation) {
  MakePreDoubleRightRotationTree();

  tree_.Add(6, 6);
  EXPECT_NODE_EQ(7, 0, IntAVLTree::kE, tree_.Root());

  tree_.Add(2, 2);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->label);
  EXPECT_EQ(5, tree_.Root()->left->key);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->left->label);
  EXPECT_EQ(3, tree_.Root()->left->left->key);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->left->left->label);
  EXPECT_EQ(2, tree_.Root()->left->left->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->left->left->label);

  tree_.Add(1, 1);
  EXPECT_EQ(7, tree_.Root()->key);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->label);

  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->left->label);
  EXPECT_EQ(5, tree_.Root()->left->key);

  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->left->label);
  EXPECT_EQ(2, tree_.Root()->left->left->key);

  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->left->left->label);
  EXPECT_EQ(1, tree_.Root()->left->left->left->key);

  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->left->right->label);
  EXPECT_EQ(3, tree_.Root()->left->left->right->key);
}

// TEST_F(AVLTreeTest, InsertMany) {
//   const int kN = 1000;
//   for (int i = 1; i <= kN; i++) {
//     tree_.Add(i, i);
//     ASSERT_TRUE(tree_.IsBalanced());
//   }
// }

TEST_F(AVLTreeTest, Regression) {
  tree_.Add(2, 2);
  EXPECT_NODE_EQ(2, 0, IntAVLTree::kE, tree_.Root());

  tree_.Add(4, 4);
  EXPECT_NODE_EQ(2, 0, IntAVLTree::kR, tree_.Root());
  EXPECT_NODE_EQ(4, 2, IntAVLTree::kE, tree_.Root()->right);

  tree_.Add(3, 3);
  EXPECT_NODE_EQ(3, 0, IntAVLTree::kE, tree_.Root());
  EXPECT_NODE_EQ(2, 3, IntAVLTree::kE, tree_.Root()->left);
  EXPECT_NODE_EQ(4, 3, IntAVLTree::kE, tree_.Root()->right);
  tree_.Add(1, 1);
}

// TEST_F(AVLTreeTest, Regression2) {
//   tree_.Add(364, 2);
//   tree_.Add(919, 2);
//   tree_.Add(915, 2);
//   tree_.Add(825, 2);
//   tree_.Add(560, 2);
//   tree_.Add(449, 2);
//   tree_.Add(425, 2);
//   tree_.Add(160, 2);
//   tree_.Add(409, 2);
//   tree_.Add(423, 2);
//   tree_.Add(727, 2);
//   tree_.IsBalanced();
// }

// TEST_F(AVLTreeTest, InsertRandom) {
//   srand(time(NULL));
//   const int kN = 1000;
//   for (int i = 1; i <= kN; i++) {
//     int v = rand() % 1000;
//     printf("v=%d\n", v);
//     tree_.Add(v, v);
//     ASSERT_TRUE(tree_.IsBalanced());
//   }
// }

// todo
// destructor
// delete


}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
