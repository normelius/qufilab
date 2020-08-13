#ifndef VOLATILITY_H
#define VOLATILITY_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
std::tuple<py::array_t<T>, py::array_t<T>, py::array_t<T>>
    bbands_calc(const py::array_t<T> prices, const int periods, 
            const int deviations);

template <typename T>
std::tuple<py::array_t<T>, py::array_t<T>, py::array_t<T>>
    kc_calc(const py::array_t<T> prices,
            const py::array_t<T> highs, const py::array_t<T> lows,
            const int period, const int period_atr, const int deviation);

template <typename T>
py::array_t<T> atr_calc(const py::array_t<T> prices,
        const py::array_t<T> highs, const py::array_t<T>
        lows, const int periods);

template <typename T>
py::array_t<T> cv_calc(const py::array_t<T> highs,
        const py::array_t<T> lows, const int period,
        const int smoothing_perid);

#endif
