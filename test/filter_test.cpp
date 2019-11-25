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
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
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

TEST(basic_filter, constructor)
{
    using basic_condition =
      sifter::basic_condition<sifter::comparison, sifter::eq, int, std::string>;
    using basic_filter =
         sifter::basic_filter<sifter::comparison, sifter::eq, int, std::string>;

    auto c0 = basic_condition("f", 5);
    auto f0 = basic_filter(c0);

    ASSERT_TRUE(f0.left_is_condition());
    EXPECT_FALSE(f0.left_is_filter());
    EXPECT_FALSE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());

    EXPECT_EQ(f0.oper(), basic_filter::_none);
    EXPECT_EQ(f0.left_condition(), c0);

    c0.lhs() = "g";
    EXPECT_NE(f0.left_condition(), c0);

    f0.left_condition().lhs() = "g";
    EXPECT_EQ(f0.left_condition(), c0);

    auto f1 = f0;
    ASSERT_TRUE(f1.left_is_condition());
    EXPECT_FALSE(f1.left_is_filter());
    EXPECT_FALSE(f1.right_is_condition());
    EXPECT_FALSE(f1.right_is_filter());

    EXPECT_EQ(f1.oper(), basic_filter::_none);
    EXPECT_EQ(f1.left_condition(), f0.left_condition());

    f1.left_condition().rhs() = "five";
    EXPECT_NE(f1.left_condition(), f0.left_condition());

    auto f2 = std::move(f1);
    ASSERT_TRUE(f2.left_is_condition());
    EXPECT_FALSE(f2.left_is_filter());
    EXPECT_FALSE(f2.right_is_condition());
    EXPECT_FALSE(f2.right_is_filter());

    EXPECT_EQ(f2.oper(), basic_filter::_none);

    EXPECT_FALSE(f1.left_is_condition());
    EXPECT_FALSE(f1.left_is_filter());
    EXPECT_FALSE(f1.right_is_condition());
    EXPECT_FALSE(f1.right_is_filter());
}

TEST(basic_filter, operators)
{
    using basic_condition =
      sifter::basic_condition<sifter::comparison, sifter::eq, int, std::string>;
    using basic_filter =
         sifter::basic_filter<sifter::comparison, sifter::eq, int, std::string>;

    auto c0 = basic_condition("a", 3, sifter::lt);
    auto c1 = basic_condition("name", "test", sifter::like);
    auto c2 = basic_condition("z", 4);
    auto c3 = basic_condition(5, 4, sifter::ne);

    basic_filter f0(c0);
    f0 &= c1;
    ASSERT_TRUE(f0.left_is_condition());
    EXPECT_FALSE(f0.left_is_filter());
    ASSERT_TRUE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());
    EXPECT_EQ(f0.oper(), basic_filter::_and);
    EXPECT_EQ(f0.left_condition(), c0);
    EXPECT_EQ(f0.right_condition(), c1);

    basic_filter f00 = f0;
    EXPECT_EQ(f00, f0);

    f0 |= c2;
    EXPECT_FALSE(f0.left_is_condition());
    ASSERT_TRUE(f0.left_is_filter());
    ASSERT_TRUE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());
    EXPECT_EQ(f0.oper(), basic_filter::_or);
    EXPECT_EQ(f0.left_filter(), f00);
    EXPECT_EQ(f0.right_condition(), c2);

    f00 = f0;
    EXPECT_EQ(f00, f0);

    f0 &= c3;
    EXPECT_FALSE(f0.left_is_condition());
    ASSERT_TRUE(f0.left_is_filter());
    ASSERT_TRUE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());
    EXPECT_EQ(f0.oper(), basic_filter::_and);
    EXPECT_EQ(f0.left_filter(), f00);
    EXPECT_EQ(f0.right_condition(), c3);

    EXPECT_FALSE(basic_filter());
    EXPECT_TRUE(basic_filter(f0));

    basic_filter f11(c0);
    f11 &= basic_filter();
    ASSERT_TRUE(f11.left_is_condition());
    EXPECT_FALSE(f11.left_is_filter());
    EXPECT_FALSE(f11.right_is_condition());
    EXPECT_FALSE(f11.right_is_filter());
    EXPECT_EQ(f11.oper(), basic_filter::_none);
    EXPECT_EQ(f11.left_condition(), c0);

    f11 |= basic_filter();
    ASSERT_TRUE(f11.left_is_condition());
    EXPECT_FALSE(f11.left_is_filter());
    EXPECT_FALSE(f11.right_is_condition());
    EXPECT_FALSE(f11.right_is_filter());
    EXPECT_EQ(f11.oper(), basic_filter::_none);
    EXPECT_EQ(f11.left_condition(), c0);

    basic_filter f12(c1);
    f11 &= f12;
    ASSERT_TRUE(f11.left_is_condition());
    EXPECT_FALSE(f11.left_is_filter());
    ASSERT_TRUE(f11.right_is_condition());
    EXPECT_FALSE(f11.right_is_filter());
    EXPECT_EQ(f11.oper(), basic_filter::_and);
    EXPECT_EQ(f11.left_condition(), c0);
    EXPECT_EQ(f11.right_condition(), c1);

    f11 = basic_filter(c0);
    ASSERT_TRUE(f11.left_is_condition());
    EXPECT_FALSE(f11.left_is_filter());
    EXPECT_FALSE(f11.right_is_condition());
    EXPECT_FALSE(f11.right_is_filter());
    EXPECT_EQ(f11.oper(), basic_filter::_none);
    EXPECT_EQ(f11.left_condition(), c0);

    f11 |= f12;
    ASSERT_TRUE(f11.left_is_condition());
    EXPECT_FALSE(f11.left_is_filter());
    ASSERT_TRUE(f11.right_is_condition());
    EXPECT_FALSE(f11.right_is_filter());
    EXPECT_EQ(f11.oper(), basic_filter::_or);
    EXPECT_EQ(f11.left_condition(), c0);
    EXPECT_EQ(f11.right_condition(), c1);

    basic_filter f13 = basic_filter(c0) && f12;
    ASSERT_TRUE(f13.left_is_condition());
    EXPECT_FALSE(f13.left_is_filter());
    ASSERT_TRUE(f13.right_is_condition());
    EXPECT_FALSE(f13.right_is_filter());
    EXPECT_EQ(f13.oper(), basic_filter::_and);
    EXPECT_EQ(f13.left_condition(), c0);
    EXPECT_EQ(f13.right_condition(), c1);

    basic_filter f14 = basic_filter(c0) || f12;
    ASSERT_TRUE(f14.left_is_condition());
    EXPECT_FALSE(f14.left_is_filter());
    ASSERT_TRUE(f14.right_is_condition());
    EXPECT_FALSE(f14.right_is_filter());
    EXPECT_EQ(f14.oper(), basic_filter::_or);
    EXPECT_EQ(f14.left_condition(), c0);
    EXPECT_EQ(f14.right_condition(), c1);

    basic_filter f15 = basic_filter(c0) && c1;
    ASSERT_TRUE(f15.left_is_condition());
    EXPECT_FALSE(f15.left_is_filter());
    ASSERT_TRUE(f15.right_is_condition());
    EXPECT_FALSE(f15.right_is_filter());
    EXPECT_EQ(f15.oper(), basic_filter::_and);
    EXPECT_EQ(f15.left_condition(), c0);
    EXPECT_EQ(f15.right_condition(), c1);

    basic_filter f16 = basic_filter(c0) || c1;
    ASSERT_TRUE(f16.left_is_condition());
    EXPECT_FALSE(f16.left_is_filter());
    ASSERT_TRUE(f16.right_is_condition());
    EXPECT_FALSE(f16.right_is_filter());
    EXPECT_EQ(f16.oper(), basic_filter::_or);
    EXPECT_EQ(f16.left_condition(), c0);
    EXPECT_EQ(f16.right_condition(), c1);

    basic_filter f17 = f15 && f16;
    EXPECT_FALSE(f17.left_is_condition());
    ASSERT_TRUE(f17.left_is_filter());
    EXPECT_FALSE(f17.right_is_condition());
    ASSERT_TRUE(f17.right_is_filter());
    EXPECT_EQ(f17.oper(), basic_filter::_and);
    EXPECT_EQ(f17.left_filter(), f15);
    EXPECT_EQ(f17.right_filter(), f16);

    basic_filter f18 = f15 || f16;
    EXPECT_FALSE(f18.left_is_condition());
    ASSERT_TRUE(f18.left_is_filter());
    EXPECT_FALSE(f18.right_is_condition());
    ASSERT_TRUE(f18.right_is_filter());
    EXPECT_EQ(f18.oper(), basic_filter::_or);
    EXPECT_EQ(f18.left_filter(), f15);
    EXPECT_EQ(f18.right_filter(), f16);

    basic_filter f19 = f15;
    f19 &= f16;
    EXPECT_FALSE(f19.left_is_condition());
    ASSERT_TRUE(f19.left_is_filter());
    EXPECT_FALSE(f19.right_is_condition());
    ASSERT_TRUE(f19.right_is_filter());
    EXPECT_EQ(f19.oper(), basic_filter::_and);
    EXPECT_EQ(f19.left_filter(), f15);
    EXPECT_EQ(f19.right_filter(), f16);

    f19 = f15;
    f19 |= f16;
    EXPECT_FALSE(f19.left_is_condition());
    ASSERT_TRUE(f19.left_is_filter());
    EXPECT_FALSE(f19.right_is_condition());
    ASSERT_TRUE(f19.right_is_filter());
    EXPECT_EQ(f19.oper(), basic_filter::_or);
    EXPECT_EQ(f19.left_filter(), f15);
    EXPECT_EQ(f19.right_filter(), f16);
}

TEST(basic_filter, comparison)
{
    using basic_condition =
      sifter::basic_condition<sifter::comparison, sifter::eq, int, std::string>;
    using basic_filter =
         sifter::basic_filter<sifter::comparison, sifter::eq, int, std::string>;

    auto c0 = basic_condition("f", 5);
    auto f0 = basic_filter(c0);
    auto f1 = basic_filter(c0);

    EXPECT_EQ(f0, f1);
    EXPECT_FALSE(f0 != f1);
}