#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <string_view>
#include <array>
#include <charconv>
#include <deque>
#include <algorithm>
#include <unordered_set>
#include <iomanip>
#include <memory>

#include "lib.h"

struct policy
{
    enum class reallocation
    {
        linear,
        pool
    };
    
    struct strategy 
    {
        template<size_t Total_Count>
        struct fixed
        {
            size_t operator()(size_t value) const 
            {
                return value;
            }

            size_t initial_count() const 
            {
                return Total_Count;
            }


        };

        template<size_t Initial_Count>
        struct expand_doubling
        {
            size_t operator()(size_t value) const 
            {
                return value * 2U;
            }

            size_t initial_count() const 
            {
                return Initial_Count;
            }
        };
    };
};

template<class T, class Tstrategy, policy::reallocation Treallocation>
class myalloc
{
    using symbol = typename T;
    using word   = symbol[8U];

    // using container_t = std::conditional<
    //                         std::is_same<Treallocation, policy::reallocation::linear>::value,
    //                         std::vector<symbol>, std::list<word>>;

    using strategy_t = policy::strategy::fixed<16> ;// Tstrategy;

    strategy_t strategy;

    std::unique_ptr<symbol[]> __memptr {nullptr};
    std::deque<bool>          __empty_memcells;
    
    size_t                    __total_count;
    size_t                    __used_count;

    std::unique_ptr<symbol[]> alloc(size_t count)
    {
        auto memptr = reinterpret_cast<symbol*>(malloc(sizeof(symbol) * count));

        memset(memptr, 0, sizeof(symbol) * __total_count);

        return memptr;
    }

    void setup_memory(std::unique_ptr<symbol[]>&& mempool, size_t mempool_size)
    {
        assert(__memptr);
        assert(mempool_size);

        __memptr = std::move(mempool);
        __total_count = mempool_size;
        __empty_memcells.resize(__total_count, true);
    }

public:
    myalloc()
    {
        __total_count = strategy.initial_count();
        __used_count = 0;

        auto new_memptr = alloc(__total_count);

        setup_memory(std::move(new_memptr), __total_count);
    }

    T* allocate(size_t count)
    {
        assert(count > 0);

        // count of T should be emplaced linear 
        //if constexpr (Treallocation == policy::reallocation::linear)
        //{
            symbol *memptr = reinterpret_cast<symbol*>(__memptr);
            symbol *subsequence_memptr = memptr;
            symbol *longest_memptr = nullptr;
            size_t  subsequence_length = 0;
            size_t  longest_subsequence_length = 0;

            for(const auto is_cell_empty : __empty_memcells)
            {
                if (!is_cell_empty)
                {
                    if (subsequence_length > longest_subsequence_length)
                    {
                        longest_subsequence_length = subsequence_length;
                        longest_memptr = subsequence_memptr;
                    }
                    
                    subsequence_length = 0;
                    subsequence_memptr = std::next(memptr);
                }

                subsequence_length += static_cast<size_t>(is_cell_empty);
                ++memptr;
            }

            if (subsequence_length > longest_subsequence_length)
            {
                longest_subsequence_length = subsequence_length;
                longest_memptr = subsequence_memptr;
            }

            // have to realloc
            if (longest_subsequence_length < count)
            {
                throw std::bad_alloc;
            }

            return longest_memptr;
        //}
    }

    void deallocate(void* memory_ptr, std::size_t size)
    {
        if ( (!memory_ptr) || (size == 0u) )
        {
            return;
        }

        symbol* deallocation_ptr = reinterpret_cast<symbol*>(memory_ptr);

        //if ()

        // TO DO

    }

};

void test()
{
    myalloc<int, policy::strategy::fixed<16>, policy::reallocation::linear> alloc;
}