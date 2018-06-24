cmake_minimum_required(VERSION 3.8)
project(Bignum)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Source/main.cpp src/Source/Bignum.cpp src/Header/Bignum.h  src/Source/Util.cpp src/Header/Util.h src/Source/BignumUtil.cpp src/Header/BignumUtil.h)
add_executable(Bignum ${SOURCE_FILES})