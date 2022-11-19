project(AlgoLab2)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDART 14)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
	AlgoLab2
	Array.h
	ArrayTest.cpp
)

target_link_libraries(
  AlgoLab2
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(AlgoLab2)
