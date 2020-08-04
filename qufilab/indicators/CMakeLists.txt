#c++ -Xpreprocessor -fopenmp test_parallell.cc -lomp

### IMPORTANT! ###
# If the project depends on multiple files, these needs to be
# included in the dependencies.


# Global instructions 
cmake_minimum_required(VERSION 3.7)
project(indicators)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(PYBIND11_CPP_STANDARD -std=c++11)
set(PYBIND11_PYTHON_VERSION 3.7)
find_package(pybind11 REQUIRED)


# Trend module
#project(trend)
pybind11_add_module(trend trend.cc)

# Volatility module
set(VOLATILITY_DEPENDENCIES trend.cc stat.cc)
#project(volatility)
pybind11_add_module(volatility volatility.cc ${VOLATILITY_DEPENDENCIES})

# Momentum module
set(MOMENTUM_DEPENDENCIES trend.cc)
#project(momentum)
pybind11_add_module(momentum momentum.cc ${MOMENTUM_DEPENDENCIES})

# Volume module
set(VOLUME_DEPENDENCIES trend.cc)
#project(volume)
pybind11_add_module(volume volume.cc ${VOLUME_DEPENDENCIES})


# Stat module
set(STAT_DEPENDENCIES trend.cc)
pybind11_add_module(stat stat.cc ${STAT_DEPENDENCIES})


#SET(GCC_COVERAGE_COMPILE_FLAGS "-mavx2")
#SET(GCC_COVERAGE_LINK_FLAGS "-lomp")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(volatility PUBLIC OpenMP::OpenMP_CXX)
endif()
