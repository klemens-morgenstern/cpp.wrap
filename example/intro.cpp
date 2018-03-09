/**
 * @file   example/intro.cpp/
 * @date   17.04.2017
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

void foo(int);

//[intro
//wrap.cpp
#include <cpp/wrap.hpp>
#include <cassert>

struct stubber
{
    int call_cnt = 0;
    int i_in = 0;

    CPP_WRAP_FN(bar, void, (int i))
    {
        i_in = i;
        call_cnt++;
    }
};

int main(int argc, char* argv[])
{
    stubber s;
    assert(s.call_cnt == 0);
    assert(s.i_in == 0);

    foo(42);

    assert(s.call_cnt == 1);
    assert(s.i_in == 42);

    return 0;
}//]
