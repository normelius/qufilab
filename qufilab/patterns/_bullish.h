#ifndef BULLISH_H
#define BULLISH_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<int> hammer_calc(py::array_t<T> opens, py::array_t<T> highs,
        py::array_t<T> lows, py::array_t<T> closes)

#endif
