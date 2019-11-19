/*
 * Copyright (c) 2019 Sergei Fundaev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <gtest/gtest.h>
#include <sifter/filter.hpp>

TEST(node, constructor)
{
    sifter::node<int> n0;
    EXPECT_FALSE(n0.condition);
    EXPECT_FALSE(n0.filter);

    sifter::condition<int, std::string> c1("a", 5);
    sifter::node<int, std::string> n1(c1);
    ASSERT_TRUE(n1.condition);
    EXPECT_FALSE(n1.filter);
    EXPECT_TRUE(*n1.condition == c1);

    c1.lhs() = "c";
    EXPECT_TRUE(*n1.condition != c1);

    sifter::node<int, std::string> n2(n1);
    EXPECT_TRUE(n2.condition);
    EXPECT_FALSE(n2.filter);
    EXPECT_TRUE(*n2.condition == *n1.condition);
    EXPECT_FALSE(n2.condition == n1.condition);

    n2.condition->comp() = sifter::condition<int, std::string>::lt;
    EXPECT_FALSE(*n2.condition == *n1.condition);

    sifter::node<int, std::string> n3(std::move(n1));
    EXPECT_TRUE(n3.condition);
    EXPECT_FALSE(n3.filter);
    EXPECT_FALSE(n1.condition);
    EXPECT_FALSE(n1.filter);
}