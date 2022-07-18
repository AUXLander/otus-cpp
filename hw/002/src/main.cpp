#include <fstream>
#include "main.h"

int main(int, char const *[])
{
    try
    {
        auto print = [](const ipv4& ip) { std::cout << ip << std::endl; };

        ip_pool_t any = get_ip_pool(std::fstream{ "path\\to\\ip_filter.tsv" });
        // ip_pool_t any = get_ip_pool(std::cin);

        {
            std::for_each(any.begin(), any.end(), print);
        }

        {
            ip_pool_t tmp;

            std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 1; });

            std::for_each(tmp.begin(), tmp.end(), print);
        }

        {
            ip_pool_t tmp;

            std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 46 && ip.bytes[1] == 70; });

            std::for_each(tmp.begin(), tmp.end(), print);
        }

        {
            ip_pool_t tmp;

            std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 46 || ip.bytes[1] == 46 || ip.bytes[2] == 46 || ip.bytes[3] == 46; });

            std::for_each(tmp.begin(), tmp.end(), print);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
