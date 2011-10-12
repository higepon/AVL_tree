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

class AVLTreeTest : public ::testing::Test {
 protected:
  IntAVLTree tree_;
 public:
  AVLTreeTest()  {
  }

  virtual ~AVLTreeTest() {
  }
};


TEST_F(AVLTreeTest, AddOneNode) {
  tree_.Add(1, 1);
  Node* root = tree_.Root();
  EXPECT_TRUE(Node(1, 1, NULL, NULL, NULL, IntAVLTree::kE) == *root);
}

TEST_F(AVLTreeTest, AddTwoNodesRightSide) {
  tree_.Add(3, 3);
  tree_.Add(5, 5);
  Node* root = tree_.Root();
  EXPECT_EQ(IntAVLTree::kR, root->label);
  EXPECT_EQ(3, root->key);
  Node* child = root->right;
  ASSERT_TRUE(child != NULL);
  EXPECT_EQ(IntAVLTree::kE, child->label);
  EXPECT_EQ(5, child->key);
}

TEST_F(AVLTreeTest, AddThreeNodesRightSide) {
  tree_.Add(3, 3);
  tree_.Add(5, 5);
  tree_.Add(7, 7);
  Node* root = tree_.Root();
  EXPECT_EQ(IntAVLTree::kE, root->label);
  EXPECT_EQ(5, root->key);
  Node* right = root->right;
  Node* left = root->left;
  ASSERT_TRUE(right != NULL);
  ASSERT_TRUE(left != NULL);
  EXPECT_EQ(IntAVLTree::kE, left->label);
  EXPECT_EQ(IntAVLTree::kE, right->label);
  EXPECT_EQ(7, right->key);
  EXPECT_EQ(3, left->key);
}

TEST_F(AVLTreeTest, DoubleRightRotation) {
  tree_.Add(5, 5);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->label);

  tree_.Add(3, 3);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->label);
  EXPECT_EQ(3, tree_.Root()->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->label);

  tree_.Add(9, 9);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->label);
  EXPECT_EQ(3, tree_.Root()->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->label);
  EXPECT_EQ(9, tree_.Root()->right->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->right->label);

  tree_.Add(7, 7);
  EXPECT_EQ(IntAVLTree::kR, tree_.Root()->label);
  EXPECT_EQ(3, tree_.Root()->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->label);
  EXPECT_EQ(9, tree_.Root()->right->key);
  EXPECT_EQ(IntAVLTree::kL, tree_.Root()->right->label);
  EXPECT_EQ(7, tree_.Root()->right->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->right->left->label);

  tree_.Add(10, 10);
  EXPECT_EQ(IntAVLTree::kR, tree_.Root()->label);
  EXPECT_EQ(3, tree_.Root()->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->left->label);
  EXPECT_EQ(9, tree_.Root()->right->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->right->label);
  EXPECT_EQ(7, tree_.Root()->right->left->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->right->left->label);
  EXPECT_EQ(10, tree_.Root()->right->right->key);
  EXPECT_EQ(IntAVLTree::kE, tree_.Root()->right->right->label);

  // Node* root = tree_.Root();
  // EXPECT_EQ(IntAVLTree::kR, root->label);
  // EXPECT_EQ(5, root->key);
  // Node* left = root->left;
  // ASSERT_TRUE(left != NULL);
  // EXPECT_EQ(3, left->key);
  // EXPECT_EQ(IntAVLTree::kE, left->label);

  // Node* right = root->right;
  // ASSERT_TRUE(right != NULL);
  // EXPECT_EQ(9, right->key);
  // EXPECT_EQ(IntAVLTree::kE, right->label);
  // ASSERT_TRUE(right->left != NULL);
  // EXPECT_EQ(7, right->left->key);
  // EXPECT_EQ(IntAVLTree::kE, right->left->label);
  // ASSERT_TRUE(right->right != NULL);
  // EXPECT_EQ(7, right->right->key);
  // EXPECT_EQ(IntAVLTree::kE, right->right->label);

  // kokomade setup
  // kokode  6 を入れると DoubleRightRotation になるはず
}



// TEST_F(AVLTreeTest, AddTwoNodes) {
//   tree_.Add(1, 1);
//   tree_.Add(2, 2);
//   EXPECT_EQ(-1, tree_.BalanceFactor());
//   EXPECT_TRUE(tree_.IsBalanced());
// }

// TEST_F(AVLTreeTest, AddThreeNodes) {
//   tree_.Add(1, 1);
//   tree_.Add(2, 2);
//   tree_.Add(3, 3);
//   EXPECT_EQ(0, tree_.BalanceFactor());
//   EXPECT_TRUE(tree_.IsBalanced());
// }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
