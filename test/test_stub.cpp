/**
 * @file   test/stub_test.cpp
 * @date   17.03.2017
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

#include <mw/wrap.hpp>

#include <vector>

namespace std
{
extern template class std::vector<int>;
}

struct test_t
{
    static int foo(int value);
    static int bar(int value);
};


MW_WRAP_FN_FIX((), no_scope, int, (int i))
{
    return i - 42;
}

MW_WRAP_FN_FIX(fn_fix_test, (std::vector<int, std::allocator<int>>), (const std::vector<int> & i))
{
    return {i.begin(), i.end()};
}

MW_WRAP_FN_FIX((my, scope_dingens), fn_fix_test, int, (int i))
{
    return i + 42;
}

int push_res = 0;
MW_WRAP_MEM_FIX(std::vector<int>, push_back, void, (int && i))
{
    push_res = i;
}

MW_WRAP_MEM_FIX(const std::vector<int>, size, std::size_t, () )
{
    return push_res;
}


/*MW_WRAP_STATIC_MEM_FIX(test_t, foo, int, (int value))
{

}

struct stubber
{
    MW_WRAP_FN(fn_test, int, (int i))
    {

    }

    MW_WRAP_MEM((std::vector<int, std::allocator<int>>), push_back, void, (int && i))
    {

    }
    MW_WRAP_STATIC_MEM(test_t, bar, int, (int value))
    {

    }
};


void test_func()
{
    stubber s;
}
*/
