# message(STATUS "-------" ${Boost_SYSTEM_LIBRARY})

cmake_minimum_required (VERSION 2.8.11)
set(PROJECT_NAME AsioServer)
project("${PROJECT_NAME}")

# set C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
# // set C++11 support


file(GLOB SRCS "src/*.h" "src/*.cpp")

message(STATUS "-------" ${SRCS})

add_executable("${PROJECT_NAME}" ${SRCS})

add_subdirectory(src/server)

target_link_libraries(
  "${PROJECT_NAME}" 
  server
)