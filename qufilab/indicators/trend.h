#ifndef TREND_H
#define TREND_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<T> sma_calc(const py::array_t<T> price, const int period);

py::array_t<double> ema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> dema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> tema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> t3_calc(const py::array_t<double> prices, const int periods,
        const double volume_factor);

py::array_t<double> tma_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> smma_calc(const py::array_t<double> prices, const int periods);

template <typename T>
py::array_t<T> lwma_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> wc_calc(const py::array_t<T> prices, 
        const py::array_t<T> highs,
        const py::array_t<T> lows);

#endif
