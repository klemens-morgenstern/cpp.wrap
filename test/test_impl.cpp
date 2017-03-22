/**
 * @file  test_impl.cpp
 * @date   21.03.2017
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

#include <vector>

namespace std
{
template class std::vector<int>;
}

int no_scope(int value) {return value + 42;}
std::vector<int> fn_fix_test(const std::vector<int> & vec) {return {vec.rbegin(), vec.rend()};}

namespace my { namespace scope_dingens {

int fn_fix_test(int i) {return i - 42;}

}}
