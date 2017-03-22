/**
 * @file   test_stubbed.cpp
 * @date   22.03.2017
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
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>

int no_scope(int value);
std::vector<int> fn_fix_test(const std::vector<int> & vec);

namespace my { namespace scope_dingens {

int fn_fix_test(int i);
}}


BOOST_AUTO_TEST_CASE(unstubbed)
{
    BOOST_CHECK_EQUAL(no_scope(5), 47);
    BOOST_CHECK_EQUAL(my::scope_dingens::fn_fix_test(47), 5);

    std::vector<int> vec = {1,2,3,4,5,6};
    auto res = fn_fix_test(vec);

    BOOST_CHECK_EQUAL_COLLECTIONS(
                res.begin(), res.end(),
                vec.rbegin(), vec.rend());
}

BOOST_AUTO_TEST_CASE(stubbed)
{
    BOOST_CHECK_EQUAL(no_scope(47), 5);
    BOOST_CHECK_EQUAL(my::scope_dingens::fn_fix_test(5), 47);

    std::vector<int> vec = {1,2,3,4,5,6};
    auto res = fn_fix_test(vec);

    BOOST_CHECK_EQUAL_COLLECTIONS(
                res.begin(), res.end(),
                vec.begin(), vec.end());

}
