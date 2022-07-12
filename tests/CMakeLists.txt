cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://codeload.github.com/google/googletest/zip/refs/tags/release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests tests.cpp)

enable_testing()
add_test(NAME tests COMMAND tests)

target_link_libraries(tests 
    PRIVATE helloworld
    PUBLIC gtest 
)

target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR})

if (MSVC)

    target_compile_options(tests PRIVATE
        /W4
    )

else()

    target_compile_options(tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS tests DESTINATION bin)