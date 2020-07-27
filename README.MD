# My practice of modern C++

## Purpose

Stay at home doing nothing is boring!

As a ex-C++ expert for twenty years, I'm seeking something to rebuild my essential technique abilities.

Combined modern C++, classic algorithm, deep neural network and some engineering tools/framework, I made this draft plan for myself.

## Architecture

Phase 1:

Using classic algorithm to check and solve SUDUKO problem

Phase 2:

Using C++ API to call some OCR or TABLE algorithm to auto input SUDUKO into system

And use phase 1 code to solve the problem

Phase 3:

Using classic algorithm as a discriminator to train a SUDUKO generator, just like GAN

## Classic SUDUKO solve algorithm choice

1. Dancing Link smart_pointer version
2. Sparse Matrix version

## Practice some technique BTW

- CMake
- Catch2    new open source unit test framework for C++
- microsoft/vcpkg   C++ lib/source/header dependency package manager just like npm for js or maven for java

0. Install vcpkg and configure settings.json in VSCode workspace
```
{
    "cmake.configureSettings": {
      "CMAKE_TOOLCHAIN_FILE": "~/repos/vcpkg/scripts/buildsystems/vcpkg.cmake"
    }
}
```

1. Install Boost Test using vcpkg
```
vcpkg install boost-test
```
There is NO hint like 
```
The package catch2:x64-linux provides CMake targets:

    find_package(Catch2 CONFIG REQUIRED)
    target_link_libraries(main PRIVATE Catch2::Catch2)
```
2. CMake failed to find the BoostTest library
```
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
```

3. Install Catch2 using vcpkg
```
vcpkg install catch2
```
There are hints 
```
The package catch2:x64-linux provides CMake targets:

    find_package(Catch2 CONFIG REQUIRED)
    target_link_libraries(main PRIVATE Catch2::Catch2)
```
4. In test/CMakeLists.txt
```
find_package(Catch2 REQUIRED)
add_executable(Test test.cpp)
target_link_libraries(Test PRIVATE Catch2::Catch2)
include(Catch)
catch_discover_tests(Test)
```

In test/test.cpp

```
#include "catch.hpp"
```

In CMakeLists.txt of project root folder
```
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

5. Search a lot a web page, helpless except one
https://stackoverflow.com/questions/54279898/how-to-integrate-catch2-as-external-library-with-cmake?noredirect=1
It said that replace #include "catch.hpp" with <catch2/catch.hpp>

```
#include<catch2/catch.hpp>
```
It's compiled!!!  I don't know why and what the hell is the prefix of header file <catch2/catch.hpp>

But CTest still can not recognize any tests, I try to modify CMakeLists.txt, replace
```
ADD_SUBDIRECTORY(test)
INCLUDE(CTest)
ENABLE_TESTING()
```
with
```
INCLUDE(CTest)
ENABLE_TESTING()
ADD_SUBDIRECTORY(test)
```

It worked!

6. Normally usage of cmake find_package
```
find_package(<NAME> [REQUIRED]) 
include_directories(${NAME_INCLUDE_DIRS})
target_link_libraries(xxx ${NAME_LIBRARIES})
```

7. Lessen learned
- Add test sub dir after INCLUDE(CTest) and ENABLE_TESTING()
- Not all of the PackageName.cmake or PackageNameConfig.cmake will provide ${PackageName_INCLUDE_DIRS} and ${PackageName_LIBRARIES}
- Some header-only libs like Catch2, cmake can find it by find_package, but it' NOT nessasrily to include_directory(${}), just use #include <packageName/xxxx.h>

## Stupid trys, lessen learned
- clang 11, cmake, VS Build Tools and VSCode on Win10     build successful, but IntelliSense NOT working! Search web and try every solution, finally the official site said that this is a knowed issue since 2019 May and it's still open

- using smart pointers to build classical dancing link is a stupid idea! Because a object holds by a smart pointer will be automically deleted after reference count down to zero, some nodes will be deleted when we perform a cover operation on a col header, that is what we definitely don't excepted behivier.

