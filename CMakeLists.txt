cmake_minimum_required(VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)

target_include_directories(helloworld
    PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_libraries(helloworld_cli PRIVATE
    helloworld
)

add_subdirectory(tests)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )

    target_compile_options(helloworld PRIVATE
        /W4
    )

else()

    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT de.nikolaev@mfisoft.com)

include(CPack)
