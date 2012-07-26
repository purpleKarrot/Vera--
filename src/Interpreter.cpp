//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Interpreter.h"
#include "SourceLines.h"
#include "Tokens.h"
#include "globals.hpp"
#include "file.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <boost/foreach.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>

extern "C"
{
#include "lualib.h"
}

namespace luabind
{

template<typename T>
struct default_converter<std::vector<T>> : native_converter_base<std::vector<T>>
{
    static int compute_score(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TTABLE ? 0 : -1;
    }

    std::vector<T> from(lua_State* L, int index)
    {
        std::vector<T> list;

        for (luabind::iterator i(luabind::object(luabind::from_stack(L, index))), end; i != end; ++i)
            list.push_back(luabind::object_cast<T>(*i));

        return list;
    }

    void to(lua_State* L, const std::vector<T>& vector)
    {
        luabind::object list = luabind::newtable(L);

        for (std::size_t i = 0; i < vector.size(); ++i)
            list[i + 1] = vector[i];

        list.push(L);
    }
};

template<typename T>
struct default_converter<std::vector<T> const&>
	: default_converter<std::vector<T>>
{
};

} // namespace luabind

namespace Vera
{

void report_(const std::string& file, int line, const std::string& msg)
{
	vera::problems.push_back(vera::problem(file, line, msg));
}

const std::vector<std::string>& get_files()
{
    return vera::input_files;
}

void Interpreter::execute(const std::string& name)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luabind::open(L);

    luabind::module(L, "vera")
    [
        luabind::def("input_files", &get_files, luabind::return_stl_iterator),
        luabind::def("get_lines", &SourceLines::getAllLines, luabind::return_stl_iterator),
        luabind::def("report", &report_),
        luabind::def("line_count", &SourceLines::getLineCount),
        luabind::def("get_line", &SourceLines::getLine),

        luabind::class_<vera::token>("token")
            .def_readonly("value", &vera::token::value_)
            .def_readonly("line", &vera::token::line_)
            .def_readonly("column", &vera::token::column_)
            .def_readonly("name", &vera::token::name_),

        luabind::class_<vera::file>("file")
            .def_readonly("path", &vera::file::path)
            .def_readonly("line_count", &vera::file::line_count)
            ,

        luabind::def("get_tokens", &Tokens::getTokens)
    ];

    boost::filesystem::path fileName(vera::root_dir / "scripts" / (name + ".lua"));
    if (luaL_dofile(L, fileName.string().c_str()))
    {
        throw std::runtime_error(lua_tostring(L, lua_gettop(L)));
    }

    lua_close(L); // TODO: fix leak!
}

} // namespace Vera
