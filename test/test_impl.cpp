/**
 * @file  test_impl.cpp
 * @date   21.03.2017
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

#include <vector>

namespace std
{
template class std::vector<int>;
template class std::vector<double>;
}

int no_scope(int value) {return value + 42;}
std::vector<int> fn_fix_test(const std::vector<int> & vec) {return {vec.rbegin(), vec.rend()};}

namespace my { namespace scope_dingens {

int fn_fix_test(int i) {return i - 42;}

}}

int fn_test(int val)
{
    return val - 24;
}

namespace scope {
namespace sc2 {

int test(std::vector<int> & vec)
{
    return vec.at(1);
}

}
}


struct test_t
{
    static int foo(int value);
    static int bar(int value);
};

int test_t::foo(int value)
{
    return value * 2;
}

int test_t::bar(int value)
{
    return value * 3;
}
