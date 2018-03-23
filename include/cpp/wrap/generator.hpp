/**
 * @file   cpp/wrap/generator.hpp
 * @date   20.03.2017
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */
#ifndef CPP_WRAP_GENERATOR_HPP_
#define CPP_WRAP_GENERATOR_HPP_

#include <cpp/wrap/outline.hpp>
#include <set>

namespace cpp {
namespace wrap {

struct generator
{
    outline::storage &st;
    bool indirect;
    bool include_std_except;
    std::set<std::string> sym_table;

    std::string make_fn_fix        (const outline::entry & e);
    std::string make_mem_fix       (const outline::entry & e);
    std::string make_static_mem_fix(const outline::entry & e);

    std::string make_fn        (const outline::entry & e);
    std::string make_mem       (const outline::entry & e);
    std::string make_static_mem(const outline::entry & e);

    std::string make();

    const std::string wraps() const {return _wrap_store;};

    std::string _wrap_store;

    void add_wrap(const std::string &name)
    {
        if (indirect )
            _wrap_store += "-Wl,--wrap=_" + name + " ";
        else
            _wrap_store += "--wrap=_" + name + " ";

    }

};

} /* namespace wrap */
} /* namespace cpp */

#endif /* INCLUDE_CPP_WRAP_GENERATOR_HPP_ */
