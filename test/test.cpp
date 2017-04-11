/**
 * @file   test_stubbed.cpp
 * @date   22.03.2017
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
  <pre>
    /  /|  (  )   |  |  /
   /| / |   \/    | /| /
  / |/  |   /\    |/ |/
 /  /   |  (  \   /  |
                )
 </pre>
 */
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>

namespace std
{
extern template class vector<int>;
}

int no_scope(int value);
std::vector<int> fn_fix_test(const std::vector<int> & vec);

namespace my { namespace scope_dingens {

int fn_fix_test(int i);
}}

struct test_t
{
    static int foo(int value);
    static int bar(int value);
};


BOOST_AUTO_TEST_CASE(unstubbed)
{
    BOOST_CHECK_EQUAL(no_scope(5), 47);
    BOOST_CHECK_EQUAL(my::scope_dingens::fn_fix_test(47), 5);

    std::vector<int> vec = {1,2,3,4,5,6};
    auto res = fn_fix_test(vec);

    BOOST_CHECK_EQUAL_COLLECTIONS(
                res.begin(), res.end(),
                vec.rbegin(), vec.rend());

    std::vector<int> v2;
    BOOST_CHECK_EQUAL(v2.size(), 0u);
    v2.push_back(42);
    BOOST_CHECK_EQUAL(v2.size(), 1u);
    BOOST_CHECK_EQUAL(test_t::foo(42), 84);

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

    std::vector<int> v2;
    BOOST_CHECK_EQUAL(v2.size(), 0u);
    v2.push_back(42);
    BOOST_CHECK_EQUAL(v2.size(), 42u);
    BOOST_CHECK_EQUAL(test_t::foo(42), 126);
}
