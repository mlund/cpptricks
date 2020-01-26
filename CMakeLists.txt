cmake_minimum_required(VERSION 3.11.4)

project(cpptricks CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(FetchContent)

# fetch range-v3
FetchContent_Declare(
    rangev3
    URL "https://github.com/ericniebler/range-v3/archive/0.10.0.tar.gz"
    URL_HASH MD5=2a68ca385f70d62398365cc2923d0573)

FetchContent_GetProperties(rangev3)
if(NOT rangev3_POPULATED)
    FetchContent_Populate(rangev3)
endif()

# fetch eigen
FetchContent_Declare(
    eigen
    URL "http://bitbucket.org/eigen/eigen/get/3.3.7.tar.gz"
    URL_HASH MD5=f2a417d083fe8ca4b8ed2bc613d20f07)
FetchContent_GetProperties(eigen)
if(NOT eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()
 

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
include_directories(SYSTEM ${doctest_SOURCE_DIR} ${rangev3_SOURCE_DIR}/include ${eigen_SOURCE_DIR})

set(hdrs
    ${CMAKE_SOURCE_DIR}/pairwise_iterator.h)

add_executable(test test.cpp ${hdrs})
