cmake_minimum_required (VERSION 2.8.11)
set(PROJECT_NAME AsioServer)
project("${PROJECT_NAME}")


set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.59.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

file(GLOB SRCS "src/*.h" "src/*.cpp")

add_executable("${PROJECT_NAME}" ${SRCS})

target_link_libraries("${PROJECT_NAME}" ${Boost_SYSTEM_LIBRARY})