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

#ifndef SIFTER_BASIC_FILTER_HPP
#define SIFTER_BASIC_FILTER_HPP

#ifdef SIFTER_USE_BOOST_VARIANT
#include <boost/variant2/variant.hpp>
#else
#include <variant>
#endif
#include <memory>

namespace sifter
{
#ifdef SIFTER_USE_BOOST_VARIANT
    template <typename... T>
    using variant = boost::variant2::variant<T...>;

    template <typename T, typename... V>
    constexpr T& get(const variant<V...> &v)
    {
        return boost::variant2::get<T>(v);
    }

    template <typename T, typename... V>
    constexpr T& get(variant<V...> &v)
    {
        return boost::variant2::get<T>(v);
    }
#else
    template <typename... T>
    using variant = std::variant<T...>;

    template <typename T, typename... V>
    constexpr T& get(const variant<V...> &v)
    {
        return std::get<T>(v);
    }

    template <typename T, typename... V>
    constexpr T& get(variant<V...> &v)
    {
        return std::get<T>(v);
    }
#endif

    enum class operation
    {
        _none,
        _and,
        _or
    };

    template<typename Comparison, Comparison def_value, typename... Types>
    class basic_filter;

    template<typename Comparison, Comparison def_value, typename... Types>
    class basic_condition
    {
    public:
#ifdef SIFTER_USE_BOOST_VARIANT
    using value_type = variant<Types...>;
#else
        using value_type = std::variant<Types...>;
#endif
        using filter_type = basic_filter<Comparison, def_value, Types...>;

    public:
        explicit basic_condition(const value_type &lhs = value_type(),
                                 const value_type &rhs = value_type(),
                                 Comparison c = def_value)
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

        const value_type &lhs() const
        {
            return m_lhs;
        }

        value_type &lhs()
        {
            return m_lhs;
        }

        const value_type &rhs() const
        {
            return m_rhs;
        }

        value_type &rhs()
        {
            return m_rhs;
        }

        const Comparison &comp() const
        {
            return m_operator;
        }

        Comparison &comp()
        {
            return m_operator;
        }

        bool operator==(const basic_condition &c) const
        {
            return (m_lhs == c.m_lhs && m_rhs == c.m_rhs &&
                    m_operator == c.m_operator);
        }

        bool operator!=(const basic_condition &c) const
        {
            return (m_lhs != c.m_lhs || m_rhs != c.m_rhs ||
                    m_operator != c.m_operator);
        }

        basic_condition &operator=(const basic_condition &c)
        {
            m_lhs = c.m_lhs;
            m_rhs = c.m_rhs;
            m_operator = c.m_operator;
            return *this;
        }

        filter_type operator&&(const basic_condition &c) const
        {
            return (filter_type(*this) && c);
        }

        filter_type operator&&(const filter_type &f) const
        {
            return (filter_type(*this) && f);
        }

        filter_type operator||(const basic_condition &c) const
        {
            return (filter_type(*this) || c);
        }

        filter_type operator||(const filter_type &f) const
        {
            return (filter_type(*this) || f);
        }

    private:
        value_type m_lhs;
        value_type m_rhs;
        Comparison m_operator = def_value;
    };

    template<typename Comparison, Comparison def_value, typename... Types>
    struct basic_node;

    template<typename Comparison, Comparison def_value, typename... Types>
    class basic_filter
    {
    public:
        using condition_type = basic_condition<Comparison, def_value, Types...>;
        using node_type = basic_node<Comparison, def_value, Types...>;

    public:
        basic_filter() = default;

        basic_filter(const basic_filter &f)
                : m_lhs(f.m_lhs),
                  m_rhs(f.m_rhs),
                  m_operator(f.m_operator)
        {
        }

        basic_filter(basic_filter &&f) noexcept
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

        basic_filter &operator=(const basic_filter &f)
        {
            m_lhs = f.m_lhs;
            m_rhs = f.m_rhs;
            m_operator = f.m_operator;
            return *this;
        }

        basic_filter &operator=(basic_filter &&f) noexcept
        {
            m_lhs = std::move(f.m_lhs);
            m_rhs = std::move(f.m_rhs);
            m_operator = f.m_operator;
            return *this;
        }

        basic_filter &operator=(const condition_type &c)
        {
            m_lhs = c;
            m_rhs.reset();
            m_operator = operation::_none;
        }

        basic_filter &operator=(condition_type &&c)
        {
            m_lhs = std::move(c);
            m_rhs.reset();
            m_operator = operation::_none;
        }

        bool operator==(const basic_filter &f) const
        {
            return (
                    m_lhs == f.m_lhs &&
                    m_rhs == f.m_rhs &&
                    m_operator == f.m_operator
            );
        }

        bool operator!=(const basic_filter &f) const
        {
            return (
                    m_lhs != f.m_lhs ||
                    m_rhs != f.m_rhs ||
                    m_operator != f.m_operator
            );
        }

        basic_filter &operator&=(const condition_type &rhs)
        {
            if (m_operator != operation::_none)
                m_lhs = std::move(basic_filter(*this));

            m_rhs = rhs;
            m_operator = operation::_and;
            return *this;
        }

        basic_filter &operator&=(const basic_filter &rhs)
        {
            if (!rhs)
                return *this;

            if (m_operator != operation::_none)
                m_lhs = std::move(make_node(*this));

            m_rhs = std::move(make_node(rhs));
            m_operator = operation::_and;
            return *this;
        }

        basic_filter operator&&(const condition_type &c)
        {
            basic_filter out;
            out.m_lhs = std::move(make_node(*this));
            out.m_rhs = c;
            out.m_operator = operation::_and;
            return out;
        }

        basic_filter operator&&(const basic_filter &f)
        {
            basic_filter out;
            out.m_lhs = std::move(make_node(*this));
            out.m_rhs = std::move(make_node(f));
            out.m_operator = operation::_and;
            return out;
        }

        basic_filter &operator|=(const condition_type &rhs)
        {
            if (m_operator != operation::_none)
                m_lhs = std::move(make_node(*this));

            m_rhs = rhs;
            m_operator = operation::_or;
            return *this;
        }

        basic_filter &operator|=(const basic_filter &rhs)
        {
            if (!rhs)
                return *this;

            if (m_operator != operation::_none)
                m_lhs = std::move(make_node(*this));

            m_rhs = std::move(make_node(rhs));
            m_operator = operation::_or;
            return *this;
        }

        basic_filter operator||(const condition_type &c)
        {
            basic_filter out;
            out.m_lhs = std::move(make_node(*this));
            out.m_rhs = c;
            out.m_operator = operation::_or;
            return out;
        }

        basic_filter operator||(const basic_filter &f)
        {
            basic_filter out;
            out.m_lhs = std::move(make_node(*this));
            out.m_rhs = std::move(make_node(f));
            out.m_operator = operation::_or;
            return out;
        }

        operator bool() const
        {
            return (static_cast<bool>(m_lhs) || static_cast<bool>(m_rhs));
        }

        bool left_is_condition() const
        {
            return static_cast<bool>(m_lhs.condition);
        }

        bool left_is_filter() const
        {
            return static_cast<bool>(m_lhs.filter);
        }

        bool right_is_condition() const
        {
            return static_cast<bool>(m_rhs.condition);
        }

        bool right_is_filter() const
        {
            return static_cast<bool>(m_rhs.filter);
        }

        const condition_type &left_condition() const
        {
            return *m_lhs.condition;
        }

        condition_type &left_condition()
        {
            return *m_lhs.condition;
        }

        const basic_filter &left_filter() const
        {
            return *m_lhs.filter;
        }

        basic_filter &left_filter()
        {
            return *m_lhs.filter;
        }

        const condition_type &right_condition() const
        {
            return *m_rhs.condition;
        }

        condition_type &right_condition()
        {
            return *m_rhs.condition;
        }

        const basic_filter &right_filter() const
        {
            return *m_rhs.filter;
        }

        basic_filter &right_filter()
        {
            return *m_rhs.filter;
        }

        operation oper() const
        {
            return m_operator;
        }

    private:
        node_type make_node(const basic_filter &f)
        {
            if (f.oper() != operation::_none)
                return node_type(f);

            if (f.left_is_filter())
                return node_type(f.left_filter());

            return node_type(f.left_condition());
        }

    private:
        node_type m_lhs;
        node_type m_rhs;
        operation m_operator = operation::_none;
    };


    template<typename Comparison, Comparison def_value, typename... Types>
    struct basic_node
    {
        using condition_type = basic_condition<Comparison, def_value, Types...>;
        using filter_type = basic_filter<Comparison, def_value, Types...>;

        basic_node() = default;

        basic_node(const basic_node &n)
                : condition(
                n.condition ? new condition_type(*n.condition) : nullptr),
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

        explicit basic_node(condition_type &&c)
                : condition(new condition_type(std::move(c)))
        {
        }

        explicit basic_node(const filter_type &f)
                : filter(new filter_type(f))
        {
        }

        explicit basic_node(filter_type &&f)
                : filter(new filter_type(std::move(f)))
        {
        }

        void reset()
        {
            filter.reset(nullptr);
            condition.reset(nullptr);
        }

        basic_node &operator=(const basic_node &n)
        {
            filter.reset(n.filter ? new filter_type(*n.filter) : nullptr);
            condition.reset(
                    n.condition ? new condition_type(*n.condition) : nullptr);
            return *this;
        }

        basic_node &operator=(basic_node &&n) noexcept
        {
            filter = std::move(n.filter);
            condition = std::move(n.condition);
            return *this;
        }

        basic_node &operator=(const condition_type &c)
        {
            filter.reset();
            condition.reset(new condition_type(c));
            return *this;
        }

        basic_node &operator=(condition_type &&c)
        {
            filter.reset();
            condition.reset(new condition_type(std::move(c)));
            return *this;
        }

        basic_node &operator=(const filter_type &f)
        {
            condition.reset();
            filter.reset(new filter_type(f));
            return *this;
        }

        basic_node &operator=(filter_type &&f)
        {
            condition.reset();
            filter.reset(new filter_type(std::move(f)));
            return *this;
        }

        bool operator==(const basic_node &n) const
        {
            if (static_cast<bool>(condition) != static_cast<bool>(n.condition))
                return false;

            if (static_cast<bool>(filter) != static_cast<bool>(n.filter))
                return false;

            if (condition && *condition != *n.condition)
                return false;

            return (!filter || *filter == *n.filter);
        }

        bool operator!=(const basic_node &n) const
        {
            return !(*this == n);
        }

        operator bool() const
        {
            return (static_cast<bool>(condition) || static_cast<bool>(filter));
        }

        std::unique_ptr<condition_type> condition;
        std::unique_ptr<filter_type> filter;
    };

}

#endif //SIFTER_BASIC_FILTER_HPP
