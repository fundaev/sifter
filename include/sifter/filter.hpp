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

#include <variant>
#include <memory>

namespace sifter
{

template <typename... Types>
class condition
{
public:
    using value_type = std::variant<Types...>;

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

public:
    explicit condition(const value_type &lhs = value_type(), const value_type &rhs = value_type(), comparision c = eq)
        : m_lhs(lhs),
          m_rhs(rhs),
          m_operator(c)
    {
    }
    condition(const condition &c)
        : m_lhs(c.m_lhs),
          m_rhs(c.m_rhs),
          m_operator(c.m_operator)
    {
    }
    condition(condition &&c) noexcept
        : m_lhs(std::move(c.m_lhs)),
          m_rhs(std::move(c.m_rhs)),
          m_operator(c.m_operator)
    {
    }

    const value_type& lhs() const
    {
        return m_lhs;
    }

    value_type& lhs()
    {
        return m_lhs;
    }

    const value_type& rhs() const
    {
        return m_rhs;
    }

    value_type& rhs()
    {
        return m_rhs;
    }

    const comparision& comp() const
    {
        return m_operator;
    }

    comparision& comp()
    {
        return m_operator;
    }

    condition<Types...>& operator==(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = eq;
        return *this;
    }

    condition<Types...>& operator!=(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = ne;
        return *this;
    }

    condition<Types...>& operator<(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = lt;
        return *this;
    }

    condition<Types...>& operator<=(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = le;
        return *this;
    }

    condition<Types...>& operator>(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = gt;
        return *this;
    }

    condition<Types...>& operator>=(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = ge;
        return *this;
    }

    condition<Types...>& operator%(const value_type &rhs)
    {
        m_rhs = rhs;
        m_operator = like;
        return *this;
    }

    bool operator==(const condition<Types...> &c)
    {
        return (m_lhs == c.m_lhs && m_rhs == c.m_rhs && m_operator == c.m_operator);
    }

    bool operator!=(const condition<Types...> &c)
    {
        return (m_lhs != c.m_lhs || m_rhs != c.m_rhs || m_operator != c.m_operator);
    }

    condition<Types...>& operator=(const condition<Types...> &c)
    {
        m_lhs = c.m_lhs;
        m_rhs = c.m_rhs;
        m_operator = c.m_operator;
        return *this;
    }

private:
    value_type m_lhs;
    value_type m_rhs;
    comparision m_operator = eq;
};

template <typename... Types>
struct node;

template <typename... Types>
class filter
{
public:
    using condition_type = condition<Types...>;
    using filter_type = filter<Types...>;
    using node_type = node<Types...>;

    enum operation
    {
        _none,
        _and,
        _or
    };

public:
    filter() = default;
    filter(const filter_type &f)
        : m_lhs(f.m_lhs),
          m_rhs(f.m_rhs),
          m_operator(f.m_operator)
    {
    }
    filter(filter_type &&f) noexcept
        : m_lhs(std::move(f.m_lhs)),
          m_rhs(std::move(f.m_rhs)),
          m_operator(f.m_operator)
    {
    }
    explicit filter(const condition_type &c)
        : m_lhs(c)
    {
    }
    explicit filter(condition_type &&c)
        : m_lhs(c)
    {
    }

    filter_type& operator&=(const condition_type &rhs)
    {
        if (m_operator != _none)
            m_lhs = *this;

        m_rhs = rhs;
        m_operator = _and;
    }

    filter_type& operator&=(const filter_type &rhs)
    {
        if (m_operator != _none)
            m_lhs = *this;

        m_rhs = rhs;
        m_operator = _and;
    }

    filter_type operator&&(const condition_type &c)
    {
        filter_type out;
        out.m_lhs = *this;
        out.m_rhs = c;
        out.m_operator = _and;
        return out;
    }

    filter_type operator&&(const filter_type &f)
    {
        filter_type out;
        out.m_lhs = *this;
        out.m_rhs = f;
        out.m_operator = _and;
        return out;
    }

    filter_type& operator|=(const condition_type &rhs)
    {
        if (m_operator != _none)
            m_lhs = *this;

        m_rhs = rhs;
        m_operator = _or;
    }

    filter_type& operator|=(const filter_type &rhs)
    {
        if (m_operator != _none)
            m_lhs = *this;

        m_rhs = rhs;
        m_operator = _or;
    }

    filter_type operator||(const condition_type &c)
    {
        filter_type out;
        out.m_lhs = *this;
        out.m_rhs = c;
        out.m_operator = _or;
        return out;
    }

    filter_type operator||(const filter_type &f)
    {
        filter_type out;
        out.m_lhs = *this;
        out.m_rhs = f;
        out.m_operator = _or;
        return out;
    }

private:
    node_type m_lhs;
    node_type m_rhs;
    operation m_operator = _none;
};


template<typename... Types>
struct node
{
    using condition_type = condition<Types...>;
    using filter_type = filter<Types...>;

    node() = default;

    node(const node &n)
        : condition(n.condition ? new condition_type(*n.condition) : nullptr),
          filter(n.filter ? new filter_type(*n.filter) : nullptr)
    {
    }

    node(node &&n) noexcept
        : condition(std::move(n.condition)),
          filter(std::move(n.filter))
    {
    }

    explicit node(const condition_type &c)
        : condition(new condition_type(c))
    {
    }
    explicit node(const condition_type &&c)
        : condition(new condition_type(c))
    {
    }

    explicit node(const filter_type &f)
        : filter(new filter_type(f))
    {
    }

    explicit node(const filter_type &&f)
        : filter(new filter_type(f))
    {
    }

    node& operator=(const condition_type &c)
    {
        filter.reset();
        condition.reset(new condition_type(c));
        return *this;
    }

    node& operator=(const condition_type &&c)
    {
        filter.reset();
        condition.reset(new condition_type(c));
        return *this;
    }

    node& operator=(const filter_type &f)
    {
        condition.reset();
        filter.reset(new filter_type(f));
        return *this;
    }

    node& operator=(const filter_type &&f)
    {
        condition.reset();
        filter.reset(new filter_type(f));
        return *this;
    }

    std::unique_ptr<condition_type> condition;
    std::unique_ptr<filter_type> filter;
};

}

#endif //SIFTER_FILTER_HPP
