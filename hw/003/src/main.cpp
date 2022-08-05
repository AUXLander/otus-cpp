#include <fstream>
#include <vector>
#include <list>
#include "main.h"

int main(int, char const *[])
{
    linear_allocator<int> alloc;

    // std::vector<int> vec(10, 1);
    std::vector vec({1, 2, 3, 4, 5, 6, 7, 8, 9}, alloc);

    for(auto & v : vec)
    {
        std::cout << v << std::endl;
    }

    vec.resize(20);

    return 0;
}
