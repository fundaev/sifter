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

TEST(basic_condition, constructor)
{
    using condition =
            sifter::basic_condition<sifter::comparison, sifter::gt, int>;
    auto c1 = condition();
    EXPECT_EQ(std::get<int>(c1.lhs()), 0);
    EXPECT_EQ(std::get<int>(c1.rhs()), 0);
    EXPECT_EQ(c1.comp(), sifter::gt);

    auto c2 = condition(1);
    EXPECT_EQ(std::get<int>(c2.lhs()), 1);
    EXPECT_EQ(std::get<int>(c2.rhs()), 0);
    EXPECT_EQ(c2.comp(), sifter::gt);

    auto c3 = condition(1, 3);
    EXPECT_EQ(std::get<int>(c3.lhs()), 1);
    EXPECT_EQ(std::get<int>(c3.rhs()), 3);
    EXPECT_EQ(c3.comp(), sifter::gt);

    auto c4 = condition(1, 4, sifter::ge);
    EXPECT_EQ(std::get<int>(c4.lhs()), 1);
    EXPECT_EQ(std::get<int>(c4.rhs()), 4);
    EXPECT_EQ(c4.comp(), sifter::ge);

    auto c5 =
     sifter::basic_condition<sifter::comparison, sifter::gt, int, std::string>(
             "id", 4, sifter::like);
    EXPECT_EQ(std::get<std::string>(c5.lhs()), "id");
    EXPECT_EQ(std::get<int>(c5.rhs()), 4);
    EXPECT_EQ(c5.comp(), sifter::like);
}

TEST(condition, constructor)
{
    auto c1 = sifter::condition<int>();
    EXPECT_EQ(std::get<int>(c1.lhs()), 0);
    EXPECT_EQ(std::get<int>(c1.rhs()), 0);
    EXPECT_EQ(c1.comp(), sifter::eq);

    auto c2 = sifter::condition<int>(1);
    EXPECT_EQ(std::get<int>(c2.lhs()), 1);
    EXPECT_EQ(std::get<int>(c2.rhs()), 0);
    EXPECT_EQ(c2.comp(), sifter::eq);

    auto c3 = sifter::condition<int>(1, 3);
    EXPECT_EQ(std::get<int>(c3.lhs()), 1);
    EXPECT_EQ(std::get<int>(c3.rhs()), 3);
    EXPECT_EQ(c3.comp(), sifter::eq);

    auto c4 = sifter::condition<int>(1, 4, sifter::ge);
    EXPECT_EQ(std::get<int>(c4.lhs()), 1);
    EXPECT_EQ(std::get<int>(c4.rhs()), 4);
    EXPECT_EQ(c4.comp(), sifter::ge);

    auto c5 =
         sifter::condition<int, std::string>("id", 4, sifter::like);
    EXPECT_EQ(std::get<std::string>(c5.lhs()), "id");
    EXPECT_EQ(std::get<int>(c5.rhs()), 4);
    EXPECT_EQ(c5.comp(), sifter::like);
}

TEST(condition, operators)
{
    enum field
    {
        id,
        name,
        description,
        date
    };

    using condition = sifter::condition<field, int, double, char, std::string>;

    condition c0 = condition(id) == 10;
    EXPECT_EQ(std::get<field>(c0.lhs()), id);
    EXPECT_EQ(std::get<int>(c0.rhs()), 10);
    EXPECT_EQ(c0.comp(), sifter::eq);

    auto c = condition(id) != 2.54;
    EXPECT_EQ(std::get<field>(c.lhs()), id);
    EXPECT_DOUBLE_EQ(std::get<double>(c.rhs()), 2.54);
    EXPECT_EQ(c.comp(), sifter::ne);

    c = condition(date) < -18;
    EXPECT_EQ(std::get<field>(c.lhs()), date);
    EXPECT_EQ(std::get<int>(c.rhs()), -18);
    EXPECT_EQ(c.comp(), sifter::lt);

    c = condition(date) <= 23;
    EXPECT_EQ(std::get<field>(c.lhs()), date);
    EXPECT_EQ(std::get<int>(c.rhs()), 23);
    EXPECT_EQ(c.comp(), sifter::le);

    c = condition(description) > 'a';
    EXPECT_EQ(std::get<field>(c.lhs()), description);
    EXPECT_EQ(std::get<char>(c.rhs()), 'a');
    EXPECT_EQ(c.comp(), sifter::gt);

    c = condition(description) >= 'z';
    EXPECT_EQ(std::get<field>(c.lhs()), description);
    EXPECT_EQ(std::get<char>(c.rhs()), 'z');
    EXPECT_EQ(c.comp(), sifter::ge);

    c = condition(name) % "some text";
    EXPECT_EQ(std::get<field>(c.lhs()), name);
    EXPECT_EQ(std::get<std::string>(c.rhs()), "some text");
    EXPECT_EQ(c.comp(), sifter::like);

    auto c1 = condition(id, 72, sifter::le) == 10;
    EXPECT_EQ(std::get<field>(c1.lhs()), id);
    EXPECT_EQ(std::get<int>(c1.rhs()), 10);
    EXPECT_EQ(c1.comp(), sifter::eq);

    EXPECT_TRUE(c0 == c1);
    EXPECT_FALSE(c0 != c1);

    EXPECT_FALSE(c0 == c);
    EXPECT_TRUE(c0 != c);
}

TEST(basic_condition, transformation)
{
    using condition =
      sifter::basic_condition<sifter::comparison, sifter::eq, int, std::string>;
    using filter =
         sifter::basic_filter<sifter::comparison, sifter::eq, int, std::string>;

    condition c0("a", 3);
    condition c1("b", -15, sifter::ge);

    filter f0 = c0 && c1;
    ASSERT_TRUE(f0.left_is_condition());
    EXPECT_FALSE(f0.left_is_filter());
    ASSERT_TRUE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());
    EXPECT_EQ(f0.left_condition(), c0);
    EXPECT_EQ(f0.right_condition(), c1);
    EXPECT_EQ(f0.oper(), filter::_and);

    filter f1 = c0 || c1;
    ASSERT_TRUE(f1.left_is_condition());
    EXPECT_FALSE(f1.left_is_filter());
    ASSERT_TRUE(f1.right_is_condition());
    EXPECT_FALSE(f1.right_is_filter());
    EXPECT_EQ(f1.left_condition(), c0);
    EXPECT_EQ(f1.right_condition(), c1);
    EXPECT_EQ(f1.oper(), filter::_or);

    filter f2 = c0 && f1;
    ASSERT_TRUE(f2.left_is_condition());
    EXPECT_FALSE(f2.left_is_filter());
    EXPECT_FALSE(f2.right_is_condition());
    ASSERT_TRUE(f2.right_is_filter());
    EXPECT_EQ(f2.left_condition(), c0);
    EXPECT_EQ(f2.right_filter(), f1);
    EXPECT_EQ(f2.oper(), filter::_and);

    filter f3 = c0 || f2;
    ASSERT_TRUE(f3.left_is_condition());
    EXPECT_FALSE(f3.left_is_filter());
    EXPECT_FALSE(f3.right_is_condition());
    ASSERT_TRUE(f3.right_is_filter());
    EXPECT_EQ(f3.left_condition(), c0);
    EXPECT_EQ(f3.right_filter(), f2);
    EXPECT_EQ(f3.oper(), filter::_or);
}

TEST(condition, transformation)
{
    using condition = sifter::condition<int, std::string>;
    using filter = sifter::filter<int, std::string>;

    filter f0 = condition("a") == 3 && condition("b") >= -15;
    ASSERT_TRUE(f0.left_is_condition());
    EXPECT_FALSE(f0.left_is_filter());
    ASSERT_TRUE(f0.right_is_condition());
    EXPECT_FALSE(f0.right_is_filter());
    EXPECT_EQ(f0.left_condition(), condition("a", 3));
    EXPECT_EQ(f0.right_condition(), condition("b", -15, sifter::ge));
    EXPECT_EQ(f0.oper(), filter::_and);

    filter f1 = condition("a") == 3 || condition("b") >= -15;
    ASSERT_TRUE(f1.left_is_condition());
    EXPECT_FALSE(f1.left_is_filter());
    ASSERT_TRUE(f1.right_is_condition());
    EXPECT_FALSE(f1.right_is_filter());
    EXPECT_EQ(f1.left_condition(), condition("a", 3));
    EXPECT_EQ(f1.right_condition(), condition("b", -15, sifter::ge));
    EXPECT_EQ(f1.oper(), filter::_or);

    filter f2 = condition("a") == 3 && f1;
    ASSERT_TRUE(f2.left_is_condition());
    EXPECT_FALSE(f2.left_is_filter());
    EXPECT_FALSE(f2.right_is_condition());
    ASSERT_TRUE(f2.right_is_filter());
    EXPECT_EQ(f2.left_condition(), condition("a", 3));
    EXPECT_EQ(f2.right_filter(), f1);
    EXPECT_EQ(f2.oper(), filter::_and);

    filter f3 = condition("a") == 3 || condition("a") == 3 &&
                                       (condition("a") == 3 ||
                                        condition("b") >= -15);
    ASSERT_TRUE(f3.left_is_condition());
    EXPECT_FALSE(f3.left_is_filter());
    EXPECT_FALSE(f3.right_is_condition());
    ASSERT_TRUE(f3.right_is_filter());
    EXPECT_EQ(f3.left_condition(), condition("a", 3));
    EXPECT_EQ(f3.right_filter(), f2);
    EXPECT_EQ(f3.oper(), filter::_or);
}