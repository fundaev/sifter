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

#ifndef SIFTER_OSTREAM_HPP
#define SIFTER_OSTREAM_HPP

#include <ostream>
#include "filter.hpp"

namespace sifter
{
    enum class special
    {
        condition_begin,
        condition_end,
        filter_begin,
        filter_end
    };

    template <typename Dumper, typename Comparison, Comparison def_value,
            typename... Types>
    class basic_out
    {
    public:
        using filter_type = basic_filter<Comparison, def_value, Types...>;
        using condition_type = basic_condition<Comparison, def_value, Types...>;

    public:
        explicit basic_out(const filter_type &filter)
            : m_filter(&filter),
              m_condition(nullptr)
        {
        }

        explicit basic_out(const condition_type &condition)
            : m_filter(nullptr),
              m_condition(&condition)
        {
        }

        friend std::ostream& operator<<(std::ostream& out, const basic_out &o)
        {
            Dumper dumper(out);

            if (o.m_filter)
                dump(*o.m_filter, dumper);
            else if (o.m_condition)
                dump(*o.m_condition, dumper);

            return out;
        }

    private:
        static void dump(const condition_type &c, Dumper &dumper)
        {
            dumper(special::condition_begin);
            std::visit(dumper, c.lhs());
            dumper(c.comp());
            std::visit(dumper, c.rhs());
            dumper(special::condition_end);
        }

        static void dump(const filter_type &f, Dumper &dumper)
        {
            if (!f)
                return;

            const bool left_only = f.oper() == operation::_none;

            if (!left_only)
                dumper(special::filter_begin);

            if (f.left_is_filter())
                dump(f.left_filter(), dumper);
            else if (f.left_is_condition())
                dump(f.left_condition(), dumper);

            if (!left_only)
            {
                dumper(f.oper());

                if (f.right_is_filter())
                    dump(f.right_filter(), dumper);
                else if (f.right_is_condition())
                    dump(f.right_condition(), dumper);
            }

            if (!left_only)
                dumper(special::filter_end);
        }

    private:
        const filter_type *m_filter;
        const condition_type *m_condition;
    };

    template <typename Dumper, typename... Types>
    using out = basic_out<Dumper, comparison, eq, Types...>;

    class default_dumper
    {
    public:
        explicit default_dumper(std::ostream &out)
            : m_out(out)
        {
        }

        template <typename T>
        void operator()(const T &value)
        {
            m_out << value;
        }

    private:
        std::ostream &m_out;
    };

    template<>
    void default_dumper::operator()(const special &value)
    {
        switch (value)
        {
            case special::filter_begin:
                m_out << '(';
                break;
            case special::filter_end:
                m_out << ')';
                break;
            case special::condition_begin:
            case special::condition_end:
                break;
        }
    }

    template<>
    void default_dumper::operator()(const operation &value)
    {
        switch (value)
        {
            case operation::_none:
                break;
            case operation::_and:
                m_out << "&&";
                break;
            case operation::_or:
                m_out << "||";
                break;
        }
    }

    template<>
    void default_dumper::operator()(const comparison &value)
    {
        switch (value)
        {
            case eq:
                m_out << "==";
                break;
            case ne:
                m_out << "!=";
                break;
            case lt:
                m_out << '<';
                break;
            case le:
                m_out << "<=";
                break;
            case gt:
                m_out << '>';
                break;
            case ge:
                m_out << ">=";
                break;
            case like:
                m_out << '~';
                break;
        }
    }
}

#endif //SIFTER_OSTREAM_HPP
