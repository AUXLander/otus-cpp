#include <fstream>
#include "lib.h"
#include "main.h"
#include "env.h"

#include "gtest/gtest.h"

using namespace std::string_view_literals;

TEST(test_valid_version, test_valid_version)
{
    ASSERT_TRUE(version() > 0);
}

TEST(test_ip_pool, test_ip_pool_select_first_1)
{
    std::fstream file_stream{ get_test_data_path() };

    ip_pool_t any = get_ip_pool(file_stream);
    ip_pool_t tmp;

    ip_pool_t origin = {
        "1.231.69.33"sv,
        "1.87.203.225"sv,
        "1.70.44.170"sv,
        "1.29.168.152"sv,
        "1.1.234.8"sv
    };

    std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 1; });

    ASSERT_TRUE(origin == tmp);
}

TEST(test_ip_pool, test_ip_pool_select_first_46_second_70)
{
    std::fstream file_stream{ get_test_data_path() };

    ip_pool_t any = get_ip_pool(file_stream);
    ip_pool_t tmp;

    ip_pool_t origin = {
        "46.70.225.39"sv,
        "46.70.147.26"sv,
        "46.70.113.73"sv,
        "46.70.29.76"sv
    };

    std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 46 && ip.bytes[1] == 70; });

    ASSERT_TRUE(origin == tmp);
}

TEST(test_ip_pool, test_ip_pool_select_any_46)
{
    std::fstream file_stream{ get_test_data_path() };

    ip_pool_t any = get_ip_pool(file_stream);
    ip_pool_t tmp;

    ip_pool_t origin = {
        "186.204.34.46"sv,
        "186.46.222.194"sv,
        "185.46.87.231"sv,
        "185.46.86.132"sv,
        "185.46.86.131"sv,
        "185.46.86.131"sv,
        "185.46.86.22"sv,
        "185.46.85.204"sv,
        "185.46.85.78"sv,
        "68.46.218.208"sv,
        "46.251.197.23"sv,
        "46.223.254.56"sv,
        "46.223.254.56"sv,
        "46.182.19.219"sv,
        "46.161.63.66"sv,
        "46.161.61.51"sv,
        "46.161.60.92"sv,
        "46.161.60.35"sv,
        "46.161.58.202"sv,
        "46.161.56.241"sv,
        "46.161.56.203"sv,
        "46.161.56.174"sv,
        "46.161.56.106"sv,
        "46.161.56.106"sv,
        "46.101.163.119"sv,
        "46.101.127.145"sv,
        "46.70.225.39"sv,
        "46.70.147.26"sv,
        "46.70.113.73"sv,
        "46.70.29.76"sv,
        "46.55.46.98"sv,
        "46.49.43.85"sv,
        "39.46.86.85"sv,
        "5.189.203.46"sv,
    };

    std::copy_if(any.begin(), any.end(), std::back_inserter(tmp), [](ipv4& ip) { return ip.bytes[0] == 46 || ip.bytes[1] == 46 || ip.bytes[2] == 46 || ip.bytes[3] == 46; });

    ASSERT_TRUE(origin == tmp);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
