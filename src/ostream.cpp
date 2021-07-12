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

#include <sifter/ostream.hpp>

sifter::default_dumper::default_dumper(std::ostream &out)
    : m_out(out)
{
}

template <>
void sifter::default_dumper::operator()(const sifter::special &value)
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

template <>
void sifter::default_dumper::operator()(const sifter::operation &value)
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

template <>
void sifter::default_dumper::operator()(const sifter::comparison &value)
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
