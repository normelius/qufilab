#ifndef TREND_H
#define TREND_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


py::array_t<double> sma_calc(const py::array_t<double> price, const int period);

py::array_t<double> ema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> dema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> tema_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> t3_calc(const py::array_t<double> prices, const int periods,
        const double volume_factor);

py::array_t<double> tma_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> smma_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> lwma_calc(const py::array_t<double> prices, const int periods);

py::array_t<double> wc_calc(const py::array_t<double> prices, 
        const py::array_t<double> highs,
        const py::array_t<double> lows);

#endif
