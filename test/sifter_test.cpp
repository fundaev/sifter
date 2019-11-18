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
#include "sifter/filter.hpp"

TEST(condition, constructor)
{
    auto c1 = sifter::condition<int>();
    EXPECT_EQ(std::get<int>(c1.lhs()), 0);
    EXPECT_EQ(std::get<int>(c1.rhs()), 0);
    EXPECT_EQ(c1.comp(), sifter::condition<int>::eq);

    auto c2 = sifter::condition<int>(1);
    EXPECT_EQ(std::get<int>(c2.lhs()), 1);
    EXPECT_EQ(std::get<int>(c2.rhs()), 0);
    EXPECT_EQ(c2.comp(), sifter::condition<int>::eq);

    auto c3 = sifter::condition<int>(1, 3);
    EXPECT_EQ(std::get<int>(c3.lhs()), 1);
    EXPECT_EQ(std::get<int>(c3.rhs()), 3);
    EXPECT_EQ(c3.comp(), sifter::condition<int>::eq);

    auto c4 = sifter::condition<int>(1, 4, sifter::condition<int>::ge);
    EXPECT_EQ(std::get<int>(c4.lhs()), 1);
    EXPECT_EQ(std::get<int>(c4.rhs()), 4);
    EXPECT_EQ(c4.comp(), sifter::condition<int>::ge);

    auto c5 = sifter::condition<int, std::string>("id", 4, sifter::condition<int, std::string>::like);
    EXPECT_EQ(std::get<std::string>(c5.lhs()), "id");
    EXPECT_EQ(std::get<int>(c5.rhs()), 4);
    EXPECT_EQ(c5.comp(), sifter::condition<int>::like);
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
    EXPECT_EQ(c0.comp(), condition::eq);

    auto c = condition(id) != 2.54;
    EXPECT_EQ(std::get<field>(c.lhs()), id);
    EXPECT_DOUBLE_EQ(std::get<double>(c.rhs()), 2.54);
    EXPECT_EQ(c.comp(), condition::ne);

    c = condition(date) < -18;
    EXPECT_EQ(std::get<field>(c.lhs()), date);
    EXPECT_EQ(std::get<int>(c.rhs()), -18);
    EXPECT_EQ(c.comp(), condition::lt);

    c = condition(date) <= 23;
    EXPECT_EQ(std::get<field>(c.lhs()), date);
    EXPECT_EQ(std::get<int>(c.rhs()), 23);
    EXPECT_EQ(c.comp(), condition::le);

    c = condition(description) > 'a';
    EXPECT_EQ(std::get<field>(c.lhs()), description);
    EXPECT_EQ(std::get<char>(c.rhs()), 'a');
    EXPECT_EQ(c.comp(), condition::gt);

    c = condition(description) >= 'z';
    EXPECT_EQ(std::get<field>(c.lhs()), description);
    EXPECT_EQ(std::get<char>(c.rhs()), 'z');
    EXPECT_EQ(c.comp(), condition::ge);

    c = condition(name) % "some text";
    EXPECT_EQ(std::get<field>(c.lhs()), name);
    EXPECT_EQ(std::get<std::string>(c.rhs()), "some text");
    EXPECT_EQ(c.comp(), condition::like);

    auto c1 = condition(id, 72, condition::le) == 10;
    EXPECT_EQ(std::get<field>(c1.lhs()), id);
    EXPECT_EQ(std::get<int>(c1.rhs()), 10);
    EXPECT_EQ(c1.comp(), condition::eq);

    EXPECT_TRUE(c0 == c1);
    EXPECT_FALSE(c0 != c1);

    EXPECT_FALSE(c0 == c);
    EXPECT_TRUE(c0 != c);
}