cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)
project(kt-library)

add_subdirectory(sdl2)

add_executable(kt-tests 
  tests/build-test.cpp
)
target_compile_options(build-test PRIVATE "-fsyntax-only")