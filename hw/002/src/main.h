#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <string_view>
#include <array>
#include <charconv>
#include <deque>
#include <algorithm>
#include <unordered_set>
#include <iomanip>

#include "lib.h"

/*
 * This function will split provided string with separator and call for lambda to process substring.
 * The result of lambda to check will splitting be countinued or not
 */
template< class Tlambda>
void split(const std::string_view &str, char separator, Tlambda lambda)
{
    bool result = true;
    size_t index = 0U;
    size_t begin = 0U;
    size_t end = str.find_first_of(separator, begin);

    for (; result && end != std::string::npos; ++index)
    {
        result = lambda(index, std::string_view{ str.data() + begin, end - begin });

        begin = end + 1;
        end = str.find_first_of(separator, begin);
    }

    if (result)
    {
        lambda(index, std::string_view{ str.data() + begin, str.size() - begin });
    }
}

struct ipv4
{
    std::array<unsigned char, 4U> bytes{ 0U, 0U, 0U, 0U };

public:

    struct hash
    {
        inline size_t operator()(const ipv4 &o) const noexcept
        {
            return o.__reinterpret_continuous();
        }
    };

public:

    ipv4() = default;
    ipv4(const ipv4&) = default;
    explicit ipv4(ipv4&&) noexcept = default;

    ipv4& operator=(const ipv4&) = default;

    ipv4(const std::string_view &str)
    {
        constexpr auto SEPARATOR = '.';

        split(str, SEPARATOR,
            [this](size_t index, const std::string_view &substr)
            {
                if (index < bytes.size())
                {
                    std::from_chars(substr.data(), substr.data() + substr.size(), bytes[index]);
                }

                return index < bytes.size();
            }
        );
    }

    inline bool operator>(const ipv4 &other) const noexcept
    {
        return __reinterpret_continuous() > other.__reinterpret_continuous();
    }

    inline bool operator<(const ipv4 &other) const noexcept
    {
        return __reinterpret_continuous() < other.__reinterpret_continuous();
    }

    inline bool operator==(const ipv4 &other) const noexcept
    {
        return __reinterpret_continuous() == other.__reinterpret_continuous();
    }

    void print(std::ostream &output) const
    {
        const auto store_flags = output.flags();

        output << std::fixed;

        output << std::setw(3U) << static_cast<int>(bytes[0]) << '.'
               << std::setw(3U) << static_cast<int>(bytes[1]) << '.'
               << std::setw(3U) << static_cast<int>(bytes[2]) << '.'
               << std::setw(3U) << static_cast<int>(bytes[3]);

        output.flags(store_flags);
    }

    std::string to_string() const
    {
        std::string output;

        output.reserve(15); // sso optimized

        output += static_cast<int>(bytes[0]) + '.';
        output += static_cast<int>(bytes[0]) + '.';
        output += static_cast<int>(bytes[0]) + '.';
        output += static_cast<int>(bytes[0]);

        return output;
    }

    friend std::ostream& operator<<(std::ostream &output, const ipv4 &ip)
    {
        ip.print(output);

        return output;
    }

private:

    inline uint32_t __reinterpret_continuous() const noexcept
    {
        return static_cast<uint32_t>(bytes[0]) << 24
             | static_cast<uint32_t>(bytes[1]) << 16
             | static_cast<uint32_t>(bytes[2]) << 8
             | static_cast<uint32_t>(bytes[3]) << 0;
    }
};

using ip_pool_t = std::deque<ipv4>;

template<class Tstream>
ip_pool_t get_ip_pool(Tstream &stream)
{
    ip_pool_t ip_pool;

    for (std::string line; std::getline(stream, line);)
    {
        constexpr auto SEPARATOR = '\t';

        split(line, SEPARATOR,
            [&ip_pool](size_t index, const std::string_view &substr)
            {
                ip_pool.emplace_back(substr);

                // interested in a first splitted substring
                return index == 0 ? false : false;
            }
        );
    }

    std::sort(ip_pool.begin(), ip_pool.end(), std::greater{});

    return ip_pool;
}
