# template seperate file does NOT compile!!!
Pls do not seperate code with template into header file and impl file!!!
Ridiculous error hint: 
```
xxx::xxx() reference not find
```
# smart_pointer
1. Do NOT constract more than one smart_pointer on one obj
2. std::enable_shared_from_this is a utility class helper to safely get a smart pointer like make_shared(this)
    2.1 class foo : public std::enable_shared_from_this<foo>
    {
        using std::enable_shared_from_this::shared_from_this();
    }
    2.2 IMPORTANT: every enabled_shared_from_this derived class, MUST call the shared_from_this() before its was hold by a shared_ptr

# Unit testing framework
    Google test
    Catch2  https://github.com/catchorg/Catch2
# Mock framework

# CPP package management tool
    microsoft/vspkg   https://github.com/microsoft/vcpkg/

# Someting about CMake with C++
1 Turn C++ 11,C++ 14,C++17,C++20 on in the CMakeLists.txt
```
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)
```
2 Set the C/C++ include directory in the CMakeLists.txt
```
INCLUDE_DIRECTORIES(include)
```
3 Set the output binary file directory from build/bin to bin
```
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
```

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
