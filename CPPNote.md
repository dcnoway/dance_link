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