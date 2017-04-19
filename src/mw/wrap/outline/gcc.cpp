/**
 * @file   mw/test/outline/gcc.hpp
 * @date   18.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 *
 *
 <pre>
    /  /|  (  )   |  |  /
   /| / |   \/    | /| /
  / |/  |   /\    |/ |/
 /  /   |  (  \   /  |
                )
 </pre>
 */


#include <boost/spirit/home/x3.hpp>
#include <mw/wrap/outline/gcc.hpp>
#include <iostream>

namespace mw
{
namespace wrap
{
namespace outline
{

namespace x3 = boost::spirit::x3;

struct symbol_type_r_ : x3::symbols<symbol_type>
{
    symbol_type_r_()
    {
        add ("T", symbol_type::text )
            ("t", symbol_type::text )
            ("U", symbol_type::unknown )
            ("W", symbol_type::weak )
            ("w", symbol_type::weak )
            ;
    }
} const symbol_type_r;

storage parse_gcc(const std::string & file, std::istream & fs_m, std::istream & fs_d)
{
    storage ret;
    std::string s_m;
    std::string s_d;
    int line = 0;
    while (fs_m && fs_d)
    {
        line++;
        std::getline(fs_m, s_m);
        std::getline(fs_d, s_d);

        if (s_m.empty() && s_d.empty())
            break;

        entry ole;
        char type_c;
        auto itr = s_m.begin();

        bool skip = false; //whether or not to skip this entry. That is because only text, undefined and weak are of interest here.
        auto set_address = [&](auto & c) {ole.address = x3::_attr(c);};
        auto set_type    = [&](auto & c) {ole.type    = x3::_attr(c);};
        auto set_skip    = [&](auto & c) {skip = true; type_c = x3::_attr(c);};
        auto set_mangled = [&](auto & c)
            {
                auto &st = x3::_attr(c);
                ole.mangled = std::string(st.begin()+1, st.end());
            };

        auto b = x3::parse(itr, s_m.end(),
            -x3::hex[set_address] >> +x3::space >>
            (symbol_type_r[set_type] | x3::char_("AaBbCDdGgiINnPpRrSsuVvWw")[set_skip]) >>
            x3::space >> (*(x3::char_ - x3::eol))[set_mangled] >> *x3::eol >>x3::eoi
             );

        if (!b)
        { std::cerr << file << '(' << line << ") Could not parse this line: '" << s_m << "'" << std::endl; std::exit(1);}

        auto mismatch = s_d.end(); //whether there's a mismatch.

        auto set_demangled = [&](auto & c) {ole.demangled = x3::_attr(c);};
        auto cmp_type = [&](auto &c)
            {
                if (skip || (x3::_attr(c) != ole.type))
                {
                    mismatch = x3::_where(c).begin();
                    x3::_pass(c) = false;
                }
            };
        auto cmp_skip = [&](auto & c)
            {
                if (!skip || (x3::_attr(c) != type_c))
                {
                    mismatch = x3::_where(c).begin();
                    x3::_pass(c) = false;
                }
            };
        auto cmp_address = [&](auto & c)
                {
                    if (x3::_attr(c) ? (x3::_attr(c) != *ole.address) : false )
                    {
                        mismatch = x3::_where(c).begin();
                        x3::_pass(c) = false;
                    }
                };

        itr = s_d.begin();
        b = x3::parse(itr, s_d.end(),
            -x3::hex[cmp_address] >> +x3::space >>
            (symbol_type_r[cmp_type] | x3::char_("AaBbCDdGgiINnPpRrSsuVvWw")[cmp_skip]) >>
            x3::space >> (*x3::char_)[set_demangled] >> x3::eoi
             );

        if (!b)
        {
            if (mismatch == s_d.end())
                std::cerr << file << '(' << line << "): " << "Could not parse this line.: '" << s_d << "'"<< std::endl;
            else
                std::cerr << file << '(' << line << " - " << mismatch-s_d.begin() << "): "  << "Mismatch between outline files. " << std::endl;
            std::exit(1);
        }
        if (!skip && (ole.demangled[0] != '.'))
            ret.push_back(std::move(ole));

    }
    return ret;
}



}
}
}



