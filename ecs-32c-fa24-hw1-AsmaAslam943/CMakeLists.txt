cmake_minimum_required(VERSION 3.10)
project(hello)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# This is geting gunit so you don't have to...
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(hello main.c
        dynamic_array.c
        dynamic_array.h)
	
enable_testing()


add_executable(testbinary dynamic_array.c dynamic_array_tests.cpp dynamic_array.h) 
target_link_libraries(
  testbinary
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testbinary)