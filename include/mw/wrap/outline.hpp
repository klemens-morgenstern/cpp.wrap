/*
 * outline.hpp
 *
 *  Created on: 18.02.2016
 *      Author: Klemens
 */

#ifndef MW_TEST_OUTLINE_HPP_
#define MW_TEST_OUTLINE_HPP_

#include <cstdint>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

namespace mw
{
namespace wrap
{

namespace outline
{

enum class symbol_type
{
    unknown,
    weak,
    text,
};

struct entry
{
    symbol_type type;
    boost::optional<std::uint64_t> address;
    std::string mangled;
    std::string demangled;
};

using storage = std::vector<entry>;

}}}

#endif /* MW_TEST_OUTLINE_HPP_ */
