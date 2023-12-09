cmake_minimum_required(VERSION 3.1)
project("Advent of Code 2023 - Day 1" LANGUAGES CXX)

# use c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

add_executable(part1 "")
target_sources(
	part1
	PRIVATE
		part1.cpp
)

add_executable(part2 "")
target_sources(
	part2
	PRIVATE
		part2.cpp
)
