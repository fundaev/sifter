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

#include <sstream>
#include <gtest/gtest.h>
#include <sifter/ostream.hpp>

TEST(out, default_dumper)
{
    using condition = sifter::condition<int, std::string>;
    using filter = sifter::filter<int, std::string>;
    using out = sifter::out<sifter::default_dumper, int, std::string>;

    filter f0 = condition("a") < 10 && condition("name") % "some text";

    std::stringstream b0;
    b0 << out(f0);
    EXPECT_EQ(b0.str(), "(a<10&&name~some text)");


    filter f1 = condition("a") == 10 || condition("b") != 20;
    f1 &= condition("c") >= 7;

    std::stringstream b1;
    b1 << out(f1);
    EXPECT_EQ(b1.str(), "((a==10||b!=20)&&c>=7)");


    filter f2(condition("a") == 10);
    f2 |= condition("b") != 20 && condition("c") >= 7;

    std::stringstream b2;
    b2 << out(f2);
    EXPECT_EQ(b2.str(), "(a==10||(b!=20&&c>=7))");
}