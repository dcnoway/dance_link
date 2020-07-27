find_package(Catch2 CONFIG REQUIRED)
#include_directories(${Catch2_INCLUDE_DIRS})
#message(STATUS "Catch2_INCLUDE_DIRS = ${Catch2_INCLUDE_DIRS}")
add_executable(Test test.cpp)
 
target_link_libraries(Test PRIVATE Catch2::Catch2)

include(Catch)
catch_discover_tests(Test)
