# message(STATUS "-------" ${Boost_SYSTEM_LIBRARY})

cmake_minimum_required (VERSION 2.8.11)
set(PROJECT_NAME AsioServer)
project("${PROJECT_NAME}")

# set C++11 support
# http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()
# // set C++11 support


add_executable("${PROJECT_NAME}" src/main.cpp)

add_subdirectory(src/server)

target_link_libraries("${PROJECT_NAME}" server)