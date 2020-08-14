


# Global instructions.
cmake_minimum_required(VERSION 3.7)
project(patterns)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(PYBIND11_CPP_STANDARD -std=c++11)
set(PYBIND11_PYTHON_VERSION 3.7)
find_package(pybind11 REQUIRED)

# Bullish patterns module.
set(BULLISH_DEPENDENCIES)
pybind11_add_module(_bullish _bullish.cc ${BULLISH_DEPENDENCIES})

