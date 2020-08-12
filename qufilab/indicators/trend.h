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

template <typename T>
py::array_t<T> ema_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> dema_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> tema_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> t3_calc(const py::array_t<T> prices, const int periods,
        const double volume_factor);

template <typename T>
py::array_t<T> tma_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> smma_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> lwma_calc(const py::array_t<T> prices, const int periods);

template <typename T>
py::array_t<T> wc_calc(const py::array_t<T> prices, 
        const py::array_t<T> highs,
        const py::array_t<T> lows);

#endif
