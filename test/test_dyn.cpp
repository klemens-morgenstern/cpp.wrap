/**
 * @file   test_stubbed.cpp
 * @date   22.03.2017
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>

namespace std
{
extern template class vector<int>;
extern template class vector<double>;
}

int no_scope(int value);
std::vector<int> fn_fix_test(const std::vector<int> & vec);

int fn_test(int val);
namespace my { namespace scope_dingens {

int fn_fix_test(int i);
}}


namespace scope {
namespace sc2 {

int test(std::vector<int> & vec);
}
}

struct test_t
{
    static int foo(int value);
    static int bar(int value);
};

std::unique_ptr<void, void(*)(void*)> test_func(
        const std::function<int(int)> & f1,
        std::function<void(std::vector<double> &, const double&)> f2,
        std::function<std::size_t(const std::vector<double> &)>f3,
        std::function<int(int)>    f4);


BOOST_AUTO_TEST_CASE(test)
{
    std::vector<double> vec = {1,2,3,4,5,6};
    auto capacity = vec.capacity();
    double i = 42;
    vec.push_back(i);
    BOOST_CHECK_EQUAL(vec.size(), 7u);
    BOOST_CHECK_EQUAL(fn_test(66), 42);
    std::vector<int> vi = {1,2,3};
    BOOST_CHECK_EQUAL(scope::sc2::test(vi), 2);

    BOOST_CHECK_EQUAL(test_t::bar(9), 27);
    {
        bool exec1 = false;
        bool exec2 = false;
        bool exec3 = false;
        bool exec4 = false;
        auto p = test_func(
                [&](double i)
                {
                    exec1 = true;
                    return i+24;
                },
                [&](const std::vector<double> & v,const double & j)
                {
                    exec2 = true;
                    BOOST_CHECK_EQUAL(&v, &vec);
                    BOOST_CHECK_EQUAL(j, i);
                },
                [&](const std::vector<double> & v)
                {
                    BOOST_CHECK_EQUAL(&v, &vec);
                    exec3 = true;
                    return 0xBEEF;
                },
                [&](int j)
                {
                    exec4 = true;
                    return j*j;
                });

        BOOST_CHECK_EQUAL(fn_test(42), 66);
        BOOST_CHECK_EQUAL(scope::sc2::test(vi), 1);
        i = 42;
        vec.push_back(i);
        BOOST_CHECK_EQUAL(vec.size(), 7u);
        BOOST_CHECK(exec2);
        BOOST_CHECK_EQUAL(vec.capacity(), 0xBEEF);
        BOOST_CHECK(exec3);

        BOOST_CHECK_EQUAL(test_t::bar(3), 9);
        BOOST_CHECK_EQUAL(test_t::bar(4), 16);
        BOOST_CHECK(exec4);

    }

    BOOST_CHECK_EQUAL(test_t::bar(3), 9);

    BOOST_CHECK_EQUAL(fn_test(42), 18);
    BOOST_CHECK_EQUAL(scope::sc2::test(vi), 2);

    vec.push_back(i);
    BOOST_CHECK_EQUAL(vec.size(), 8u);
    BOOST_CHECK_NE(0xBEEF, vec.capacity());
}
