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

#include <map>
#include <string>
#include <iostream>
#include <sifter/filter.hpp>

namespace sql
{
    class query
    {
    public:
        explicit query(const std::string &sql)
                : m_sql(sql)
        {
        }

        const std::string& sql() const
        {
            return m_sql;
        }

        std::string bound_sql() const
        {
            std::string sql = m_sql;
            for (const auto p : m_int_values)
                replace(sql, p.first, std::to_string(p.second));

            for (const auto p : m_string_values)
                replace(sql, p.first, "'" + p.second + "'");

            return sql;
        }

        void bind(const std::string &placeholder, int value)
        {
            m_int_values[placeholder] = value;
        }

        void bind(const std::string &placeholder, const std::string &value)
        {
            m_string_values[placeholder] = value;
        }

    private:
        void replace(std::string &target, const std::string &placeholder,
                     const std::string &value) const
        {
            const std::size_t pos = target.find(placeholder);
            if (pos != std::string::npos)
                target.replace(pos, placeholder.size(), value);
        }

    private:
        std::string m_sql;
        std::map<std::string, int> m_int_values;
        std::map<std::string, std::string> m_string_values;
    };

    enum field
    {
        id,
        name,
        age
    };

    using filter = sifter::filter<field, int, std::string>;
    using condition = sifter::condition<field, int, std::string>;
    using fieldmap = std::map<field, std::string>;

    void
    bind(query &q, const condition::value_type &v, const std::string &prefix)
    {
        const std::string placeholder = ":" + prefix + "v";
#ifdef SIFTER_USE_BOOST_VARIANT
        if (boost::variant2::holds_alternative<int>(v))
            q.bind(placeholder, boost::variant2::get<int>(v));
        else if (boost::variant2::holds_alternative<std::string>(v))
            q.bind(placeholder, boost::variant2::get<std::string>(v));
#else
        if (std::holds_alternative<int>(v))
            q.bind(placeholder, std::get<int>(v));
        else if (std::holds_alternative<std::string>(v))
            q.bind(placeholder, std::get<std::string>(v));
#endif
    }

    void
    bind(query &q, const filter::condition_type &c, const std::string &prefix)
    {
        bind(q, c.lhs(), prefix + "l");
        bind(q, c.rhs(), prefix + "r");
    }

    void bind(query &q, const filter &f, const std::string &prefix = "")
    {
        if (f.left_is_condition())
            bind(q, f.left_condition(), prefix + "l");
        else if (f.left_is_filter())
            bind(q, f.left_filter(), prefix + "l");

        if (f.right_is_condition())
            bind(q, f.right_condition(), prefix + "r");
        else if (f.right_is_filter())
            bind(q, f.right_filter(), prefix + "r");
    }

    std::string dump(sifter::comparison c)
    {
        std::string out;
        switch (c)
        {
            case sifter::eq:
                out = " = ";
                break;
            case sifter::ne:
                out = " <> ";
                break;
            case sifter::lt:
                out = " < ";
                break;
            case sifter::le:
                out = " <= ";
                break;
            case sifter::gt:
                out = " > ";
                break;
            case sifter::ge:
                out = " >= ";
                break;
            case sifter::like:
                out = " like ";
                break;
        }
        return out;
    }

    std::string dump(sifter::operation o)
    {
        std::string out;
        switch (o)
        {
            case sifter::operation::_and:
                out = " and ";
                break;
            case sifter::operation::_or:
                out = " or ";
                break;
            default:
                break;
        }
        return out;
    }

    std::string dump(const condition::value_type &v, const std::string &prefix,
                     const fieldmap &sql_fields)
    {
#ifdef SIFTER_USE_BOOST_VARIANT
        if (boost::variant2::holds_alternative<field>(v))
        {
            const auto p = sql_fields.find(boost::variant2::get<field>(v));
            return p->second;
        }
#else
        if (std::holds_alternative<field>(v))
        {
            const auto p = sql_fields.find(std::get<field>(v));
            return p->second;
        }
#endif
        return ":" + prefix + "v";
    }

    std::string dump(const filter::condition_type &c, const std::string &prefix,
                     const fieldmap &sql_fields)
    {
        return dump(c.lhs(), prefix + "l", sql_fields) + dump(c.comp()) +
               dump(c.rhs(), prefix + "r", sql_fields);
    }

    std::string
    dump(const filter &f, const std::string &prefix, const fieldmap &sql_fields)
    {
        if (!f)
            return "";

        std::string out = "(";
        if (f.left_is_condition())
            out += dump(f.left_condition(), prefix + "l", sql_fields);
        else if (f.left_is_filter())
            out += dump(f.left_filter(), prefix + "l", sql_fields);

        out += dump(f.oper());

        if (f.right_is_condition())
            out += dump(f.right_condition(), prefix + "r", sql_fields);
        else if (f.right_is_filter())
            out += dump(f.right_filter(), prefix + "r", sql_fields);

        out += ")";
        return out;
    }

    std::string where(const filter &f, const fieldmap &sql_fields)
    {
        return " where " + dump(f, "", sql_fields);
    }
}


int main(int, char**)
{
    sql::filter f = sql::condition(sql::id) < 10 &&
                    (sql::condition(sql::name) % "John Smith" ||
                     sql::condition(sql::age) > 20);

    const sql::fieldmap sql_fields =
    {
        {sql::id, "id"},
        {sql::name, "name"},
        {sql::age, "age"}
    };

    std::string text = "select * from table" + sql::where(f, sql_fields);
    sql::query q(text);
    sql::bind(q, f);

    std::cout << q.sql() << std::endl;
    std::cout << q.bound_sql() << std::endl;

    return 0;
}