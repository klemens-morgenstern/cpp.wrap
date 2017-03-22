/**
 * @file   mw/wrap/generator.hpp
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
#ifndef MW_WRAP_GENERATOR_HPP_
#define MW_WRAP_GENERATOR_HPP_

#include <mw/wrap/outline.hpp>

namespace mw {
namespace wrap {

struct generator
{
    outline::storage &st;
    bool indirect;

    std::string make_fn_fix        (const outline::entry & e);
    std::string make_mem_fix       (const outline::entry & e) {return std::string("\n//---not implemented--- ") + __PRETTY_FUNCTION__;}
    std::string make_static_mem_fix(const outline::entry & e) {return std::string("\n//---not implemented--- ") + __PRETTY_FUNCTION__;}

    std::string make_fn        (const outline::entry & e) {return std::string("\n//---not implemented--- ") + __PRETTY_FUNCTION__;}
    std::string make_mem       (const outline::entry & e) {return std::string("\n//---not implemented--- ") + __PRETTY_FUNCTION__;}
    std::string make_static_mem(const outline::entry & e) {return std::string("\n//---not implemented--- ") + __PRETTY_FUNCTION__;}

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
} /* namespace mw */

#endif /* INCLUDE_MW_WRAP_GENERATOR_HPP_ */
