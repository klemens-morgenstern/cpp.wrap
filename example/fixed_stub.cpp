/**
 * @file   /wrap/example/fixed_stub.cpp
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

//[fixed_stub
//fixed_stub.cpp
#include <cassert>
#include <mw/wrap.hpp>

MW_WRAP_FN_FIX(foo, int, ())
{
    return 42;
}

int main(int argc, char * argv[])
{
    assert(foo() == 42);
    return 0;
}
//]
