
project(models)

### IMPORTANT! ###
# Dependencies needs to be specified below.
set(MODELS_DEPENDENCIES common/time.cc indicators/_trend.cc
    indicators/_volatility.cc)

# Global instructions 
cmake_minimum_required(VERSION 3.7)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(PYBIND11_CPP_STANDARD -std=c++11)
set(PYBIND11_PYTHON_VERSION 3.7)

find_package(pybind11 REQUIRED)
pybind11_add_module(models models.cc ${MODELS_DEPENDENCIES})



