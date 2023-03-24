cmake_minimum_required(VERSION 3.20)

project(emlib VERSION 15.0.7)

set(LLVM_TARGET_TRIPLE "thumbv7m-none-eabi" CACHE STRING "llvm triple")
option(LIBCXX_INCLUDE_BENCHMARKS "Build the libc++ benchmarks and their dependencies" OFF)

add_subdirectory(compiler-rt)
add_subdirectory(libcxx)
add_subdirectory(libcxxabi)
add_subdirectory(picolibc)
