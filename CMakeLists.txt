cmake_minimum_required(VERSION 3.0.0)
project(DancingLink VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(src bin)
#add_executable(DancingLink src/main.cpp src/dancing_link.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
