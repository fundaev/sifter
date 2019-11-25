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

#ifndef SIFTER_FILTER_HPP
#define SIFTER_FILTER_HPP

#include "basic_filter.hpp"

namespace sifter
{

    enum comparison
    {
        eq,
        ne,
        lt,
        le,
        gt,
        ge,
        like
    };

    template<typename... Types>
    class condition : public basic_condition<comparison, eq, Types...>
    {
    public:
        using value_type =
                typename basic_condition<comparison, eq, Types...>::value_type;
        using basic_type = basic_condition<comparison, eq, Types...>;

    public:
        explicit condition(const value_type &lhs = value_type(),
                           const value_type &rhs = value_type(),
                           comparison c = eq)
                : basic_condition<comparison, eq, Types...>(lhs, rhs, c)
        {
        }

        condition(const condition &c)
                : basic_condition<comparison, eq, Types...>(c)
        {
        }

        condition(condition &&c) noexcept
                : basic_condition<comparison, eq, Types...>(c)
        {
        }

        bool operator==(const condition &c) const
        {
            return *(static_cast<const basic_type *>(this)) == c;
        }

        bool operator!=(const condition &c) const
        {
            return *(static_cast<const basic_type *>(this)) != c;
        }

        condition &operator=(const condition &c)
        {
            *(static_cast<basic_type *>(this)) = c;
            return *this;
        }

        condition &operator==(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = eq;
            return *this;
        }

        condition &operator!=(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = ne;
            return *this;
        }

        condition &operator<(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = lt;
            return *this;
        }

        condition &operator<=(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = le;
            return *this;
        }

        condition &operator>(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = gt;
            return *this;
        }

        condition &operator>=(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = ge;
            return *this;
        }

        condition &operator%(const value_type &rhs)
        {
            condition::rhs() = rhs;
            condition::comp() = like;
            return *this;
        }
    };

    template<typename... Types>
    using filter = basic_filter<comparison, eq, Types...>;
    template<typename... Types>
    using node = basic_node<comparison, eq, Types...>;
}

#endif //SIFTER_FILTER_HPP
