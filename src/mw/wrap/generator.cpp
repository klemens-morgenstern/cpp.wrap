/**
 * @file   mw/wrap/generator.cpp
 * @date   20.03.2017
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 <pre>
    /  /|  (  )   |  |  /
   /| / |   \/    | /| /
  / |/  |  / \    |/ |/
 /  /   | (   \   /  |
           )
 </pre>
 */
#include <mw/wrap/generator.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/join.hpp>

#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <cppformat/format.h>


namespace mw {
namespace wrap {

std::string generator::make()
{
    mw::wrap::outline::storage st_me;
    st_me.resize(st.size());

    auto itr = std::copy_if(st.cbegin(), st.cend(), st_me.begin(),
                  [](const outline::entry & e){return e.mangled.find("_mw_wrap_") != std::string::npos;});

    st_me.erase(itr, st_me.end());

    std::string value;

    for (auto & s : st_me)
    {
        //TODO: support msvc.
        if (boost::starts_with(s.demangled, "_mw_wrap_"))
        {
            //is fix
            if (boost::starts_with(s.demangled, "_mw_wrap_fn_fix_"))
                value += make_fn_fix(s);
            else if (boost::starts_with(s.demangled, "_mw_wrap_mem_fix_"))
                value += make_mem_fix(s);
            else if (boost::starts_with(s.demangled, "_mw_wrap_static_mem_fix_"))
                value += make_static_mem_fix(s);
        }
        else if (boost::starts_with(s.demangled, "mw::detail::connector_impl") && (s.demangled.find("connect_impl") != std::string::npos))
        {
            if (s.demangled.find("_mw_wrap_fn_"))
                value += make_fn(s);
            else if (s.demangled.find("_mw_wrap_mem_"))
                value += make_mem(s);
            else if (s.demangled.find("_mw_wrap_static_mem_"))
                value += make_static_mem(s);
        }
    }
    return value;
}

namespace x3 = boost::spirit::x3;

x3::rule<class parens> parens;
auto parens_def = '(' >> *(parens | (x3::char_ - x3::char_("()"))) >> ')';
BOOST_SPIRIT_DEFINE(parens);

x3::rule<class pointy_par> pointy_par;
auto pointy_par_def = '<' >> *(parens | pointy_par | (x3::char_ - x3::char_("<>()"))) >> '>';
BOOST_SPIRIT_DEFINE(pointy_par);


auto tag_type_assign = [](auto & ctx)
        {
            auto range = x3::_attr(ctx);
            x3::_val(ctx).assign(range.begin() + 1, range.end() - 1);
        };

x3::rule<class tag_type, std::string> tag_type;
auto tag_type_def = "mw::detail::tag" >> x3::raw[pointy_par][tag_type_assign] >> '*';
BOOST_SPIRIT_DEFINE(tag_type);

auto raw_assign = [](auto & ctx)
        {
            auto range = x3::_attr(ctx);
            x3::_val(ctx).assign(range.begin(), range.end());
        };

x3::rule<class type_name, std::string> type_name;
auto type_name_def = *x3::space >> x3::raw[x3::char_("A-Za-z") >> *(parens | pointy_par | (x3::char_ - x3::char_(",)")))][raw_assign] ;
BOOST_SPIRIT_DEFINE(type_name);


std::string generator::make_fn_fix(const outline::entry & e)
{


    auto assign = [](auto & ref){return [&ref](auto & ctx){ref = x3::_attr(ctx);};};
    auto push_back = [](auto & ref){return [&ref](auto & ctx){ref.push_back(x3::_attr(ctx));};};

    auto scp_size = 0u;
    std::vector<std::string> scp_val;

    std::vector<std::string> params;
    std::string return_type;
    auto rule =
            "_mw_wrap_fn_fix_" >> x3::int_[assign(scp_size)] >> "__mw__" >> ((*(x3::char_("_A-Za-z0-9") - "__mw__"))[push_back(scp_val)] % "__mw__")
            >> '(' >> tag_type[assign(return_type)] >> *(',' >> type_name[push_back(params)]) >> ')' >> *x3::space >> x3::eoi;

    auto itr = e.demangled.begin();
    auto p = x3::parse(itr, e.demangled.end(), rule);

    if (!p | (scp_size == 0u) || ((scp_size + 1) != scp_val.size()))
        throw std::runtime_error("ill-formed name " + e.demangled);

    if (scp_val.front().empty())
        scp_val.erase(scp_val.begin());


    std::string name = std::move(scp_val.back());
    scp_val.pop_back();

    //buidl the function to stub needed function

    std::string needed;
    for (auto & sc : scp_val)
        needed += sc + "::";

    needed += name;
    needed += '(';
    for (auto & p : params)
    {
        if (needed.back() != '(')
            needed += ", ";
        needed += p;
    }
    needed += ')';

    auto it = std::find_if(st.begin(), st.end(),
               [&needed](const outline::entry & e){return boost::starts_with(e.demangled, needed);});

    if (it == st.end())
        throw std::runtime_error("Could not find required function: '" + needed + "'");

    auto fm_fwd = R"__(, static_cast<{0}>(arg{1}))__";


    auto fm = R"__(

extern "C" {Return} _{Stub}(void*, {ArgIn});
extern "C" {Return} __wrap__{Target} ({ArgIn});
{Return} __wrap__{Target} ({ArgIn})
{{
    return static_cast<{Return}>(_{Stub}(nullptr {ArgFwd}));
}}

)__";

    std::string arg_in;
    std::string arg_fwd;

    int i = 0;
    for (auto arg : params)
    {
        if (!arg_in.empty())
            arg_in  += ", ";

        arg_in  += arg + " arg" + std::to_string(i);
        arg_fwd += fmt::format(fm_fwd, arg, i);
        i++;
    }

    add_wrap(it->mangled);

    return fmt::format(fm,
            fmt::arg("Return", return_type),
            fmt::arg("Stub", e.mangled),
            fmt::arg("Target", it->mangled),
            fmt::arg("ArgIn", arg_in),
            fmt::arg("ArgFwd", arg_fwd)
            );

}

} /* namespace wrap */
} /* namespace mw */
