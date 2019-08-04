# CMakeList.txt : CMake project for LongestCommonSubsequence, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(LongestCommonSequence LANGUAGES CXX VERSION 0.0.1)

find_package(Catch2 REQUIRED)

# Add source to this project's executable.
add_executable (LCS "LCS.cpp" "LCS.h")
target_link_libraries(LCS Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(LCS)

add_executable(experiments "experiments.cpp")

add_executable(backtracking "backtracking.cpp")
target_link_libraries(backtracking Catch2::Catch2)
catch_discover_tests(backtracking)


add_executable(workshops "workshops.cpp")
target_link_libraries(workshops Catch2::Catch2)
catch_discover_tests(workshops)
