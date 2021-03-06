cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_subdirectory(thirdparty/googletest)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Add the testing executable
add_executable(test ${TEST_SOURCES} src/format.cpp)
set_property(TARGET test PROPERTY CXX_STANDARD 17)
target_link_libraries(test 
    gtest_main 
)