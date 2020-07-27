# template seperate file does NOT compile!!!
Pls do not seperate code with template into header file and impl file!!!
Ridiculous error hint: 
```
xxx::xxx() reference not find
```
# smart_pointer
1. Do NOT constract more than one smart_pointer on one obj

2. std::enable_shared_from_this is a utility class helper to safely get a smart pointer like make_shared(this)
    2.1 
```C++
class foo : public std::enable_shared_from_this<foo>
{
    using std::enable_shared_from_this::shared_from_this();
}
```
    2.2 IMPORTANT: every enabled_shared_from_this derived class, MUST call the shared_from_this() before its was hold by a shared_ptr

# Unit testing framework
- GoogleTest gtest
- Catch2  https://github.com/catchorg/Catch2

# CPP package management tool
- microsoft/vspkg   https://github.com/microsoft/vcpkg/

# Someting about CMake with C++
1. Turn C++ 11,C++ 14,C++17,C++20 on in the CMakeLists.txt
```cmake
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)
```
2. Set the C/C++ include directory in the CMakeLists.txt
```cmake
INCLUDE_DIRECTORIES(include)
```
3. Set the output binary file directory from build/bin to bin
```cmake
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
```
# The first try C++ enviroment constract on Linux
## Enviroment describ
- Windows 10 insider build 20175
- WSL2 kernal 4.19.121.1
- Ubuntu 20.04
- VSCode v1.47.2
- CMake v3.16.3
- clang 11.0.0
- vcpkg 2020.06
- Catch2 v2.12.1
## Install vcpkg
```bash
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg integrate install
sudo ln ~/vcpkg/vcpkg /usr/bin/vcpkg -s
```
## configure settings.json in VSCode workspace
```json
{
    "cmake.configureSettings": {
      "CMAKE_TOOLCHAIN_FILE": "~/repos/vcpkg/scripts/buildsystems/vcpkg.cmake"
    }
}
```

## Install Boost Test using vcpkg
```bash
vcpkg install boost-test
```

## CMake failed to find the BoostTest library
```cmake
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
```

## Install Catch2 using vcpkg
```bash
vcpkg install catch2
```

The package catch2:x64-linux provides CMake targets:
```cmake
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(main PRIVATE Catch2::Catch2)
```
## In test/CMakeLists.txt
```cmake
find_package(Catch2 REQUIRED)
add_executable(Test test.cpp)
target_link_libraries(Test PRIVATE Catch2::Catch2)
include(Catch)
catch_discover_tests(Test)
```

## In test/test.cpp

```c++
#include "catch.hpp"
```

## In CMakeLists.txt of project root folder
```cmake
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(dlx VERSION 0.1.0)

SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)
INCLUDE_DIRECTORIES(include)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_LIBRARY(sudoku_link STATIC src/sudoku_link.cpp)
ADD_EXECUTABLE(dlx src/main.cpp)
TARGET_LINK_LIBRARIES(dlx sudoku_link)

ADD_SUBDIRECTORY(test)
INCLUDE(CTest)
ENABLE_TESTING()

#ADD_TEST(NAME utest1 COMMAND Test)

SET(CPACK_PROJECT_NAME ${PROJECT_NAME})
SET(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
INCLUDE(CPack)
```
Can not compile, error is "Can not find header file:catch.hpp"

## Search a lot a web page, helpless except one
- https://stackoverflow.com/questions/54279898/how-to-integrate-catch2-as-external-library-with-cmake?noredirect=1
- It said that replace #include "catch.hpp" with <catch2/catch.hpp>

```c++
#include<catch2/catch.hpp>
```
It's compiled!!!  I don't know why and what the hell is the prefix of header file <catch2/catch.hpp>

But CTest still can not recognize any tests, I try to modify CMakeLists.txt, replace
```cmake
ADD_SUBDIRECTORY(test)
INCLUDE(CTest)
ENABLE_TESTING()
```
with
```cmake
INCLUDE(CTest)
ENABLE_TESTING()
ADD_SUBDIRECTORY(test)
```

It worked!

# Normally usage of cmake find_package
```cmake
find_package(<NAME> [REQUIRED]) 
include_directories(${NAME_INCLUDE_DIRS})
target_link_libraries(xxx ${NAME_LIBRARIES})
```

# Lesson learned
- Add test sub dir after INCLUDE(CTest) and ENABLE_TESTING()
- Not all of the PackageName.cmake or PackageNameConfig.cmake will provide ${PackageName_INCLUDE_DIRS} and ${PackageName_LIBRARIES}
- Some header-only libs like Catch2, cmake can find it by find_package, but it' NOT nessasrily to include_directory(${}), just use #include <packageName/xxxx.h>

# Google Test CMake usage

   The package gtest is compatible with built-in CMake targets:
```cmake
enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(AllTestsInMain main)
```
