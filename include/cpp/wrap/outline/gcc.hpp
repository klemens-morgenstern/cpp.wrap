/**
 * @file   cpp/test/outline/gcc.hpp
 * @date   18.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 *
 *
 */

#ifndef CPP_TEST_OUTLINE_GCC_HPP_
#define CPP_TEST_OUTLINE_GCC_HPP_

#include <cpp/wrap/outline.hpp>

namespace cpp
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



#endif /* CPP_TEST_OUTLINE_GCC_HPP_ */
