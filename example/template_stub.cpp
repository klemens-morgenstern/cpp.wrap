/**
 * @file   /wrap/example/template_stub.cpp
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

//[template_stub
//template_stub.cpp
#include <vector>
#include <cassert>
#include <mw/wrap.hpp>

namespace std
{
extern template class std::vector<int, std::allocator<int>>; //declare it
}

struct stubber
{
    std::size_t size_out;
    int push_back_in;
    MW_WRAP_MEM(const std::vector<int>, size, std::size_t, ()) /*<Declaring it const is applied to the method qualifier>*/
    {
        return size_out;
    }
    MW_WRAP_MEM(std::vector<int>, push_back, void, (int&& in)) /*<only stub move push_back>*/
    {
        size_out = in;
    }
};

int main(int argc, char * argv[])
{
    std::vector<int> vec = {1,2,3};
    assert(vec.size() == 3);

    stubber s;
    s.size_out = 42;
    assert(vec.size() == 42);
    vec.push_back(3); /*<This will invoke the move-push_back>*/
    assert(s.push_back_in == 3);

    return 0;
}
//]
