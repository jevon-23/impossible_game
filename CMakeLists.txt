# Skeleton from : https://github.com/ttroy50/cmake-examples/blob/master/01-basic/B-hello-headers/CMakeLists.txt
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2)

# Set the project name
project (impossible_game)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.c
    src/block/block.c
    src/board/board.c
    src/window/window.c
    src/utils/utils.c
    src/interrupts/interrupts.c
)

# compile options
add_compile_options(-g)

# Add an executable with the above sources
add_executable(impossible_game ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(impossible_game
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        /usr/local/opt/ncurses/include
)

# Link ncurses
link_directories(/usr/local/opt/ncurses/lib)

# Link pthreads
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

target_link_libraries(impossible_game 
    PRIVATE
        ncurses
        ${CMAKE_THREAD_LIBS_INIT}

)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#       googletest
#         URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#         )
# 
# FetchContent_MakeAvailable(googletest)
# enable_testing()
# include(GoogleTest)
# 
# add_executable(
#     block_tests
#     build/testing/block_tests.cc
#     src/block/block.c
#     include/block.h
# )
# 
# target_link_libraries(
#     block_tests
#     GTest::gtest_main
# )
# 
# gtest_discover_tests(block_tests)
