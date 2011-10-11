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

class AVLTreeTest : public ::testing::Test {
 protected:
  AVLTree<int, int> tree_;
 public:
  AVLTreeTest()  {
  }

  virtual ~AVLTreeTest() {
  }
};

TEST_F(AVLTreeTest, AddOneNode) {
  tree_.Add(1, 1);
  EXPECT_EQ(0, tree_.BalanceFactor());
  EXPECT_TRUE(tree_.IsBalanced());
}

TEST_F(AVLTreeTest, AddTwoNodes) {
  tree_.Add(1, 1);
  tree_.Add(2, 2);
  EXPECT_EQ(-1, tree_.BalanceFactor());
  EXPECT_TRUE(tree_.IsBalanced());
}

TEST_F(AVLTreeTest, AddThreeNodes) {
  tree_.Add(1, 1);
  tree_.Add(2, 2);
  tree_.Add(3, 3);
  EXPECT_EQ(0, tree_.BalanceFactor());
  EXPECT_TRUE(tree_.IsBalanced());
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
