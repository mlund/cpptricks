cmake_minimum_required(VERSION 3.11.4)

project(cpptricks CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(FetchContent)

# fetch doctest
FetchContent_Declare(
    doctest
    URL "https://github.com/onqtam/doctest/archive/2.3.6.tar.gz"
    URL_MD5 298e5773f3eb9825f6e3015e8f9511ca)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    add_definitions(-DDOCTEST_CONFIG_DISABLE)
endif()
include_directories(SYSTEM ${doctest_SOURCE_DIR})

set(hdrs
    ${CMAKE_SOURCE_DIR}/pairwise_iterator.h)

add_executable(test test.cpp ${hdrs})