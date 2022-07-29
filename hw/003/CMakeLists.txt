cmake_minimum_required(VERSION 3.14)

set(LIBRARY_NAME "hw003")
set(SUBPROJECT_NAME "${LIBRARY_NAME}_cli")
set(SUBPROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TESTS_NAME "${LIBRARY_NAME}_tests")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

add_library(${LIBRARY_NAME} ${CMAKE_SOURCE_DIR}/lib.cpp)

add_executable(${SUBPROJECT_NAME} src/main.h src/main.cpp)

target_include_directories(${LIBRARY_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_libraries(${SUBPROJECT_NAME} 
    PRIVATE ${LIBRARY_NAME}
)

target_include_directories(${SUBPROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}
)

add_subdirectory(tests)

if (MSVC)
    target_compile_options(${SUBPROJECT_NAME} PRIVATE
        /W4
    )

    target_compile_options(${LIBRARY_NAME} PRIVATE
        /W4
    )

else()

    target_compile_options(${SUBPROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    target_compile_options(${LIBRARY_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS ${SUBPROJECT_NAME} RUNTIME DESTINATION bin)
