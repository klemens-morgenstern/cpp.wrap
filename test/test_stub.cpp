/**
 * @file   test/stub_test.cpp
 * @date   17.03.2017
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

#include <mw/wrap.hpp>

#include <vector>
#include <functional>
#include <memory>

namespace std
{
extern template class std::vector<int>;
extern template class std::vector<double>;
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


MW_WRAP_STATIC_MEM_FIX(test_t, foo, int, (int value))
{
    return value * 3;
}

struct stubber
{
    std::function<int(int)>    f1;
    std::function<void(std::vector<double> &, const double & i)> f2;
    std::function<std::size_t(const std::vector<double> & )>    f3;
    std::function<int(int)>    f4;
    MW_WRAP_FN(fn_test, int, (int i))
    {
        return f1(i);
    }

    MW_WRAP_FN((scope, sc2), test, int, (std::vector<int> & vec))
    {
        return vec.at(0);
    }


    MW_WRAP_MEM((std::vector<double, std::allocator<double>>), push_back, void, (const double & i))
    {
        f2(*this_, i);
    }

    MW_WRAP_MEM((const std::vector<double, std::allocator<double>>), capacity, std::size_t, ())
    {
        return f3(*this_);
    }

    MW_WRAP_STATIC_MEM(test_t, bar, int, (int value))
    {
        return f4(value);
    }
};


std::unique_ptr<void, void(*)(void*)> test_func(
        const std::function<int(int)> & f1,
        std::function<void(std::vector<double> &, const double & i)> f2,
        std::function<std::size_t(const std::vector<double> &)>f3,
        std::function<int(int)>    f4)
{
   auto p = std::make_unique<stubber>();
   p->f1 = f1;
   p->f2 = f2;
   p->f3 = f3;
   p->f4 = f4;
   return std::unique_ptr<void, void(*)(void*)> {reinterpret_cast<void*>(p.release()), +[](void * p){delete reinterpret_cast<stubber*>(p);}};

}

