/*
 * Copyright (c) 2021 Sergei Fundaev
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
#ifdef SIFTER_USE_BOOST_VARIANT
            dumper(special::condition_begin);
            boost::variant2::visit(dumper, c.lhs());
            dumper(c.comp());
            boost::variant2::visit(dumper, c.rhs());
            dumper(special::condition_end);
#else
            dumper(special::condition_begin);
            std::visit(dumper, c.lhs());
            dumper(c.comp());
            std::visit(dumper, c.rhs());
            dumper(special::condition_end);
#endif
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
        explicit default_dumper(std::ostream &out);

        template <typename T>
        void operator()(const T &value)
        {
            m_out << value;
        }

    private:
        std::ostream &m_out;
    };

    template<>
    void default_dumper::operator()(const special &value);

    template<>
    void default_dumper::operator()(const operation &value);

    template<>
    void default_dumper::operator()(const comparison &value);
}

#endif //SIFTER_OSTREAM_HPP
