
set(LIB_NAME server)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.59.0 REQUIRED)
find_package(Boost COMPONENTS system filesystem)

include_directories(${Boost_INCLUDE_DIR})

file(GLOB SRCS "*.h" "*.cpp")

add_library(${LIB_NAME} ${SRCS})

target_link_libraries(
  ${LIB_NAME}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
)