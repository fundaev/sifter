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

#ifndef SIFTER_BASIC_FILTER_HPP
#define SIFTER_BASIC_FILTER_HPP

#include <variant>
#include <memory>

namespace sifter
{

template <typename Comparision, Comparision default_comparision, typename... Types>
class basic_condition
{
public:
    using value_type = std::variant<Types...>;
    using condition_type = basic_condition<Comparision, default_comparision, Types...>;

public:
    explicit basic_condition(const value_type &lhs = value_type(), const value_type &rhs = value_type(), Comparision c = default_comparision)
        : m_lhs(lhs),
          m_rhs(rhs),
          m_operator(c)
    {
    }
    basic_condition(const basic_condition &c)
        : m_lhs(c.m_lhs),
          m_rhs(c.m_rhs),
          m_operator(c.m_operator)
    {
    }
    basic_condition(basic_condition &&c) noexcept
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

    const Comparision& comp() const
    {
        return m_operator;
    }

    Comparision& comp()
    {
        return m_operator;
    }

    bool operator==(const condition_type &c) const
    {
        return (m_lhs == c.m_lhs && m_rhs == c.m_rhs && m_operator == c.m_operator);
    }

    bool operator!=(const condition_type &c) const
    {
        return (m_lhs != c.m_lhs || m_rhs != c.m_rhs || m_operator != c.m_operator);
    }

    condition_type& operator=(const condition_type &c)
    {
        m_lhs = c.m_lhs;
        m_rhs = c.m_rhs;
        m_operator = c.m_operator;
        return *this;
    }

private:
    value_type m_lhs;
    value_type m_rhs;
    Comparision m_operator = default_comparision;
};

template<typename Comparision, Comparision default_comparision, typename... Types>
struct basic_node;

template <typename Comparision, Comparision default_comparision, typename... Types>
class basic_filter
{
public:
    using condition_type = basic_condition<Comparision, default_comparision, Types...>;
    using filter_type = basic_filter<Comparision, default_comparision, Types...>;
    using node_type = basic_node<Comparision, default_comparision, Types...>;

    enum operation
    {
        _none,
        _and,
        _or
    };

public:
    basic_filter() = default;
    basic_filter(const filter_type &f)
        : m_lhs(f.m_lhs),
          m_rhs(f.m_rhs),
          m_operator(f.m_operator)
    {
    }
    basic_filter(filter_type &&f) noexcept
        : m_lhs(std::move(f.m_lhs)),
          m_rhs(std::move(f.m_rhs)),
          m_operator(f.m_operator)
    {
    }
    explicit basic_filter(const condition_type &c)
        : m_lhs(c)
    {
    }
    explicit basic_filter(condition_type &&c)
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


template<typename Comparision, Comparision default_comparision, typename... Types>
struct basic_node
{
    using condition_type = basic_condition<Comparision, default_comparision, Types...>;
    using filter_type = basic_filter<Comparision, default_comparision, Types...>;

    basic_node() = default;

    basic_node(const basic_node &n)
        : condition(n.condition ? new condition_type(*n.condition) : nullptr),
          filter(n.filter ? new filter_type(*n.filter) : nullptr)
    {
    }

    basic_node(basic_node &&n) noexcept
        : condition(std::move(n.condition)),
          filter(std::move(n.filter))
    {
    }

    explicit basic_node(const condition_type &c)
        : condition(new condition_type(c))
    {
    }
    explicit basic_node(const condition_type &&c)
        : condition(new condition_type(c))
    {
    }

    explicit basic_node(const filter_type &f)
        : filter(new filter_type(f))
    {
    }

    explicit basic_node(const filter_type &&f)
        : filter(new filter_type(f))
    {
    }

    basic_node& operator=(const condition_type &c)
    {
        filter.reset();
        condition.reset(new condition_type(c));
        return *this;
    }

    basic_node& operator=(const condition_type &&c)
    {
        filter.reset();
        condition.reset(new condition_type(c));
        return *this;
    }

    basic_node& operator=(const filter_type &f)
    {
        condition.reset();
        filter.reset(new filter_type(f));
        return *this;
    }

    basic_node& operator=(const filter_type &&f)
    {
        condition.reset();
        filter.reset(new filter_type(f));
        return *this;
    }

    std::unique_ptr<condition_type> condition;
    std::unique_ptr<filter_type> filter;
};

}

#endif //SIFTER_BASIC_FILTER_HPP
