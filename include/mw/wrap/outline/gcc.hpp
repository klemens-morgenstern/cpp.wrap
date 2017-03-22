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
  / |/  |  / \    |/ |/
 /  /   | (   \   /  |
               )
 </pre>
 */

#ifndef MW_TEST_OUTLINE_GCC_HPP_
#define MW_TEST_OUTLINE_GCC_HPP_

#include <mw/wrap/outline.hpp>

namespace mw
{
namespace wrap
{
namespace outline
{
/**
 * https://sourceware.org/binutils/docs/binutils/nm.html
 *
 */
storage parse_gcc(const std::string & file, std::istream & mangled, std::istream & demangled);



}
}
}



#endif /* MW_TEST_OUTLINE_GCC_HPP_ */
