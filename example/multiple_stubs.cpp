/**
 * @file  /multiple_stubs.cpp
 * @date   17.04.2017
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

int foo();

//[multiple_stubs
//multiple_stubs.cpp
#include <cassert>
#include <mw/wrap.hpp>

struct stubber1
{
    MW_WRAP_FN(foo, int, ())
    {
        return 42;
    }
};

struct stubber2
{
    MW_WRAP_FN(foo, int, ())
    {
        return -1;
    }
};

int main(int argc, char * argv[])
{
    assert(foo() == 0);
    try
    {
        stubber1 s; /*<Connect the first stub.>*/
        assert(foo() == 42); /*<Check that the stub works>*/
        stubber2 s2; /*<This throws an exception, becaues it is already connects>*/
    }
    catch(std::runtime_error&) {}

    stubber2 s; /*<Since the first one is destructed we now can use the other one>*/
    assert(foo() == -1); /*<Check if it works>*/
    return 0;
}
//]
