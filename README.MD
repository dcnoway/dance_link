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


## Stupid trys, lessen learned
- clang 11, cmake, VS Build Tools and VSCode on Win10     build successful, but IntelliSense NOT working! Search web and try every solution, finally the official site said that this is a knowed issue since 2019 May and it's still open

- using smart pointers to build classical dancing link is a stupid idea! Because a object holds by a smart pointer will be automically deleted after reference count down to zero, some nodes will be deleted when we perform a cover operation on a col header, that is what we definitely don't excepted behivier.

