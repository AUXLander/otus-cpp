#include <iostream>

#include "lib.h"

int main(int, const char*[])
{
    std::cout << "Hello world" << std::endl;
    std::cout << "Version: " << version() << std::endl;

    return 0;
}