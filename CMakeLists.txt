cmake_minimum_required(VERSION 3.1)

project(travis-test-project CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

if (${BUILD_TYPE} STREQUAL "coverage")
  
  set(CMAKE_BUILD_TYPE DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

elseif (${BUILD_TYPE} STREQUAL "debug")

  set(CMAKE_BUILD_TYPE DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

elseif (${BUILD_TYPE} STREQUAL "release")
  
  set(CMAKE_BUILD_TYPE RELEASE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O6")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

endif()

add_subdirectory(src)
add_subdirectory(test)

