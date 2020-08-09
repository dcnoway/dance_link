# The self-redemption notes of a out-man coder who barely coding for Twenty years 

## Language choose

C++: as my prefer language, which have a very clear memory structures and control abilities helps to get a solid understand for the modern language features.

Python: for scripting language and kids educations

Java: too many java programmers, forget it. Just get knowing about popular Java framework and Java platforms at a architect  level.

TypeScript: strong-type JavaScript with basic OO support, front-end practice language with node.js and electron lib.

# template separate file does NOT compile!!!

My first day recall c++ skills like a idiot, acting like a rookie...

Pls do not separate code with template into header file and implement file!!!
Ridiculous error hint: 

```
xxx::xxx() reference not find
```
# smart_pointer
1. Do NOT construct more than one smart_pointer on one obj

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
# old-fashion usage
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)

# modern usage
SET_TARGET_PROPERTIES(<YOUR TARGET> PROPERTIES        CXX_STANDARD 20        CXX_STANDARD_REQUIRED YES        CXX_EXTENSIONS NO)
```
2. Set the C/C++ include directory in the CMakeLists.txt
```cmake
INCLUDE_DIRECTORIES(include)
```
3. Set the output binary file directory from build/bin to out
```cmake
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)
```
# The first try C++ environment construct on Linux
## Environment describe
- Windows 10 insider build 20175
- WSL2 kernel 4.19.121.1
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
- Some header-only libs like Catch2, cmake can find it by find_package, but it' NOT necessarily to include_directory(${}), just use #include <packageName/xxxx.h>
- If find_package not working try to set CMAKE_MODULE_PATH env var
- If it still not working try to set VCPKG_ROOT env var, and run vcpkg integrate install

# Google Test CMake usage

   The package gtest is compatible with built-in CMake targets:
```cmake
enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(AllTestsInMain main)
```

# C++ runtime dynamic mechanism
## dynamic_cast and dynamic_pointer_cast
To cast a smart pointer of dynamic type objects, use dynamic_pointer_cast.
dynamic_pointer_cast do NOT break smart pointer's use_count.
Do NOT get raw pointer from a smart pointer and cast the raw pointer!!!
```C++
        virtual const cell_ptr get_cell(const coords_type& cds)const noexcept{
            auto itr = _elements.find(cds);
            if(itr != _elements.end()){
                if(itr->second->_type == ELEMENT_TYPE::CELL)
                    return std::dynamic_pointer_cast<cell_t<CELL_VALUE_TYPE,AXIS_TYPE,DIMENSION_NUM>>(itr->second);
                else return nullptr;
            }
            else return nullptr;
        }

```
## typeid
typeid operator returns the runtime type_info of a object

https://en.cppreference.com/w/cpp/language/typeid
```C++
const std::type_info& ti1 = typeid(A);
const std::type_info& ti2 = typeid(A);
 
assert(&ti1 == &ti2); // not guaranteed
assert(ti1.hash_code() == ti2.hash_code()); // guaranteed
assert(std::type_index(ti1) == std::type_index(ti2)); // guaranteed
```
https://docs.microsoft.com/en-us/cpp/cpp/type-info-class?view=vs-2019
```C++
class type_info {
public:
    type_info(const type_info& rhs) = delete; // cannot be copied
    virtual ~type_info();
    size_t hash_code() const;
    _CRTIMP_PURE bool operator==(const type_info& rhs) const;
    type_info& operator=(const type_info& rhs) = delete; // cannot be copied
    _CRTIMP_PURE bool operator!=(const type_info& rhs) const;
    _CRTIMP_PURE int before(const type_info& rhs) const;
    size_t hash_code() const noexcept;
    _CRTIMP_PURE const char* name() const;
    _CRTIMP_PURE const char* raw_name() const;
};
```
example for typeid usage on dynamic types
```C++
animal* a = new cat; // animal has to have at least one virtual function
animal * b;
if( typeid(*a) == typeid(cat) )
{
    // the object is of type cat! but the pointer is base pointer.
}
if(typeid(a) == typeid(b))
{
    // the pointer is of type animal
}
```

## decltype
decltype is a compile-time operator returns the type of a expression
```C++
decltype(0xdeedbeef) number = 0; // number is of type int!
decltype(someArray[0]) element = someArray[0];
```

##  Compiler template errors

Some STL template function and class do not act as your think, one tiny mistakes will raise lots of complicated errors, i.e. 

```C++
auto ptr = std::make_shared<OneTemplatedType<TemplateParam>>({init_val1,init_val2},init_val);
```

This line may cause lots of errors, actually, just add one type hint to the params is all done.

```C++
auto ptr = std::make_shared<OneTemplatedType<TemplateParam>>(SomeType{init_val1,init_val2},init_val);
```

## Using clang on windows

Although you may use clang on windows command line very well, vscode loos like did not work well on IntelliSense. Do NOT search baidu.com or bing.com or stackoverflow.com. Install the official clangd extension for vscode

```
llvm-vs-code-extensions.vscode-clangd
```

## template parameters not only as typename, but also be able to play as a expressions

```c++
template<typename T, int buf_size>
class xxxxxx
{
	std::array<T,buf_size> buf;
    int len = buf_size;
}
```

## templated class relationships

see comments in the code blocks below.

```c++
template <typename T>
class Base
{
    T m_val;
    public:
    ~Base()=default;
}
template <typename T>
class Derive: public Base<T>
{
    ...
}

int main()
{
    Base<int> * pbase = nullptr;
    Derive<int> * pint = new Derive<int>;
    Derive<long> * plong = new Derive<long>;
    pbase = pint;	//Ok
    pbase = plong;	//Chould not compile
    /****
    Look at the ASM code address labels
    The label of Base<int> is something like _Base_template_int
    The label of Base<long> is something like _Base_template_long
    Same template but different classes
    You may think template is something like a preprocessor. i.e. ANSI C MACRO  ASSERT()
    ****/
    
}

```

## code comments as documents

See https://www.doxygen.nl  for details, 

## VSCode configuration

.vscode/extensions.json

```json
{
	// See https://go.microsoft.com/fwlink/?LinkId=827846 to learn about workspace recommendations.
	// Extension identifier format: ${publisher}.${name}. Example: vscode.csharp

	// List of extensions which should be recommended for users of this workspace.
	"recommendations": [
		//C++ language support
		"ms-vscode.cpptools",
		//CMake build system support
		"ms-vscode.cmake-tools",
		//Unit tests support
		"hbenl.vscode-test-explorer",
		"matepek.vscode-catch2-test-adapter",
		//Comment documentation support for Doxygen
		"cschlosser.doxdocgen",
		//todos lists support
		"gruntfuggly.todo-tree",
		//Git lens
		"eamodio.gitlens",
        //clangd
        "llvm-vs-code-extensions.vscode-clangd"
	],
	// List of extensions recommended by VS Code that should not be recommended for users of this workspace.
	"unwantedRecommendations": [
		
	]
}
```





