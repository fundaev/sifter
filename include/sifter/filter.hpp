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

#ifndef SIFTER_FILTER_HPP
#define SIFTER_FILTER_HPP

#include "basic_filter.hpp"

namespace sifter
{

enum comparision
{
    eq,
    ne,
    lt,
    le,
    gt,
    ge,
    like
};

template <typename... Types>
class condition : public basic_condition<comparision, eq, Types...>
{
public:
    using value_type = typename basic_condition<comparision, eq, Types...>::value_type;
    using condition_type = condition<Types...>;

public:
    explicit condition(const value_type &lhs = value_type(), const value_type &rhs = value_type(), comparision c = eq)
        : basic_condition<comparision, eq, Types...>(lhs, rhs, c)
    {
    }

    condition(const condition_type &c)
        : basic_condition<comparision, eq, Types...>(c)
    {
    }

    condition(condition_type &&c) noexcept
        : basic_condition<comparision, eq, Types...>(c)
    {
    }

    condition_type& operator==(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = eq;
        return *this;
    }

    condition_type& operator!=(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = ne;
        return *this;
    }

    condition_type& operator<(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = lt;
        return *this;
    }

    condition_type& operator<=(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = le;
        return *this;
    }

    condition_type& operator>(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = gt;
        return *this;
    }

    condition_type& operator>=(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = ge;
        return *this;
    }

    condition_type& operator%(const value_type &rhs)
    {
        condition_type::rhs() = rhs;
        condition_type::comp() = like;
        return *this;
    }

    bool operator==(const condition_type &c) const
    {
        return (condition_type::lhs() == c.lhs() && condition_type::rhs() == c.rhs() &&
                condition_type::comp() == c.comp());
    }

    bool operator!=(const condition_type &c) const
    {
        return (condition_type::lhs() != c.lhs() || condition_type::rhs() != c.rhs() ||
                condition_type::comp() != c.comp());
    }

    condition_type& operator=(const condition_type &c)
    {
        condition_type::lhs() = c.lhs();
        condition_type::rhs() = c.rhs();
        condition_type::comp() = c.comp();
        return *this;
    }
};

template<typename... Types>
using filter = basic_filter<comparision, eq, Types...>;
template<typename... Types>
using node = basic_node<comparision, eq, Types...>;

}

#endif //SIFTER_FILTER_HPP
