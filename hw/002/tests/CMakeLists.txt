cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://codeload.github.com/google/googletest/zip/refs/tags/release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(TEST_DATA_IPVPOOL "R\"(${SUBPROJECT_DIR}/tests/ip_filter.tsv)\"")

configure_file(testenv.h.in ${CMAKE_BINARY_DIR}/testenv.h)

add_library(env env.cpp ${CMAKE_BINARY_DIR}/testenv.h)
add_executable(${TESTS_NAME} tests.cpp)

enable_testing()
add_test(NAME tests COMMAND tests)

target_include_directories(env
    PUBLIC ${CMAKE_SOURCE_DIR}
    PUBLIC ${CMAKE_BINARY_DIR}
)

target_include_directories(${TESTS_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}
    PUBLIC ${SUBPROJECT_DIR}/src
)

target_link_libraries(${TESTS_NAME} 
    PRIVATE ${LIBRARY_NAME}
    PUBLIC gtest 
    PUBLIC env
)

if (MSVC)

    target_compile_options(${TESTS_NAME} PRIVATE
        /W4
    )

else()

    target_compile_options(${TESTS_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS ${TESTS_NAME} DESTINATION bin)