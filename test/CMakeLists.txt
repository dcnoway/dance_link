#SET(Boost_DIR /usr/share/vcpkg/packages/boost-test_x64-linux/)
find_package(Boost COMPONENTS  unit_test_framework REQUIRED)
#find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
 
include_directories(include ${Boost_INCLUDE_DIRS})
 
add_definitions(-DBOOST_TEST_DYN_LINK -g)
add_executable(Test boosttest.cpp)

ADD_TEST(NAME MyTest COMMAND Test)
 
target_link_libraries(Test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#target_link_libraries(Test ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})