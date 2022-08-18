#pragma once

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
#include <limits>
#include <cstring>

#include "lib.h"

constexpr static bool debug = false;

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif // !__PRETTY_FUNCTION__


template<typename T>
class linear_allocator
{
public:
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;
    using reference = T&;
    using const_reference = const T&;
    using value_type = T;

    template <typename U>
    struct rebind
    { 
        using other = linear_allocator<U>;
    };

//private:
    std::byte  *__memptr { nullptr };
    std::size_t __offset { 0U };

    constexpr static std::size_t WORD_SIZE = sizeof(value_type);
    constexpr static std::size_t WORD_COUNT = 1024U;
    constexpr static std::size_t MEMORY_LIMIT_SIZE = WORD_SIZE * WORD_COUNT;
public:
    linear_allocator() noexcept
    {
        debug && std::cout << "constructor: " << __PRETTY_FUNCTION__ << std::endl;
    };

    template <class U> 
    linear_allocator(const linear_allocator<U>& other) noexcept :
        __memptr(other.__memptr), __offset(other.__offset)
    {
        debug && std::cout << "copy constructor: " << __PRETTY_FUNCTION__ << std::endl;
    }

    ~linear_allocator() noexcept
    {
        debug && std::cout << "destructor: " << __PRETTY_FUNCTION__ << std::endl;
    }

    pointer allocate(size_type count, const void* hint = 0)
    {
        debug && std::cout << "allocate: " << __PRETTY_FUNCTION__ << std::endl;

        assert(count > 0U);

        const auto size = count * WORD_SIZE;

        if (__offset + size > MEMORY_LIMIT_SIZE || hint)
        {
            throw std::bad_alloc();
        }

        if (!__memptr)
        {
            __memptr = reinterpret_cast<std::byte*>(malloc(MEMORY_LIMIT_SIZE));
            __offset = 0;

            if (__memptr)
            {
                memset(__memptr, 0, MEMORY_LIMIT_SIZE);
            }
            else
            {
                throw std::bad_alloc();
            }
        }

        auto memp = __memptr + __offset;

        __offset += size;

        return reinterpret_cast<pointer>(memp);
    }

    void deallocate(pointer const, size_type)
    {
        debug && std::cout << "deallocate: " << __PRETTY_FUNCTION__ << std::endl;

        if (__memptr)
        {
            free(__memptr);
            __memptr = nullptr;
            __offset = 0;
        }

        return;
    }

    template<typename ...Args>
    void construct(pointer const p, Args&& ...args)
    {
        debug && std::cout << "construct: " << __PRETTY_FUNCTION__ << std::endl;
        ::new (p) T(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U * const p)
    {
        debug && std::cout << "destroy: " << __PRETTY_FUNCTION__ << std::endl;
        p->~U();
    }
};

template <class T, class U>
constexpr bool operator== (const linear_allocator<T>& lhs, const linear_allocator<U>& rhs) noexcept
{
    return lhs.__memptr == rhs.__memptr;
}

template <class T, class U>
constexpr bool operator!= (const linear_allocator<T>& lhs, const linear_allocator<U>& rhs) noexcept
{
    return lhs.__memptr != rhs.__memptr;
}


template<typename T>
class pool_allocator
{
public:
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;
    using reference = T&;
    using const_reference = const T&;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = pool_allocator<U>;
    };

    //private:
    std::byte* __memptr{ nullptr };
    std::size_t __offset{ 0U };

    constexpr static std::size_t WORD_SIZE = sizeof(value_type);
    constexpr static std::size_t WORD_COUNT = 1024U;
    constexpr static std::size_t MEMORY_LIMIT_SIZE = WORD_SIZE * WORD_COUNT;
public:
    pool_allocator() noexcept
    {
        debug && std::cout << "constructor: " << __PRETTY_FUNCTION__ << std::endl;
    };

    template <class U>
    pool_allocator(const pool_allocator<U>& other) noexcept :
        __memptr(other.__memptr), __offset(other.__offset)
    {
        debug&& std::cout << "copy constructor: " << __PRETTY_FUNCTION__ << std::endl;
    }

    ~pool_allocator() noexcept
    {
        debug && std::cout << "destructor: " << __PRETTY_FUNCTION__ << std::endl;
    }

    pointer allocate(size_type count, const void* hint = 0)
    {
        debug && std::cout << "allocate: " << __PRETTY_FUNCTION__ << std::endl;

        assert(count > 0U);

        const auto size = count * WORD_SIZE;

        if (__offset + size > MEMORY_LIMIT_SIZE || hint)
        {
            throw std::bad_alloc();
        }

        if (!__memptr)
        {
            __memptr = reinterpret_cast<std::byte*>(malloc(MEMORY_LIMIT_SIZE));
            __offset = 0;

            if (__memptr)
            {
                memset(__memptr, 0, MEMORY_LIMIT_SIZE);
            }
            else
            {
                throw std::bad_alloc();
            }
        }

        auto memp = __memptr + __offset;

        __offset += size;

        return reinterpret_cast<pointer>(memp);
    }

    void deallocate(pointer const, size_type)
    {
        debug && std::cout << "deallocate: " << __PRETTY_FUNCTION__ << std::endl;

        if (__memptr)
        {
            free(__memptr);
            __memptr = nullptr;
            __offset = 0;
        }

        return;
    }

    template<typename ...Args>
    void construct(pointer const p, Args&& ...args)
    {
        debug && std::cout << "construct: " << __PRETTY_FUNCTION__ << std::endl;

        ::new (p) T(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* const p)
    {
        debug && std::cout << "destroy: " << __PRETTY_FUNCTION__ << std::endl;
        p->~U();
    }
};

template <class T, class U>
constexpr bool operator== (const pool_allocator<T>& lhs, const pool_allocator<U>& rhs) noexcept
{
    return lhs.__memptr == rhs.__memptr;
}

template <class T, class U>
constexpr bool operator!= (const pool_allocator<T>& lhs, const pool_allocator<U>& rhs) noexcept
{
    return lhs.__memptr != rhs.__memptr;
}