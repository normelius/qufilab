#ifndef VOLATILITY_H
#define VOLATILITY_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

std::tuple<py::array_t<double>, py::array_t<double>, py::array_t<double>>
    bbands_calc(const py::array_t<double> prices, const int periods, 
            const int deviations);

std::tuple<py::array_t<double>, py::array_t<double>, py::array_t<double>>
    kc_calc(const py::array_t<double> prices,
            const py::array_t<double> highs, const py::array_t<double> lows,
            const int period, const int period_atr, const int deviation);

py::array_t<double> std_calc(const py::array_t<double> prices, 
        const int periods, const bool normalize);

py::array_t<double> atr_calc(const py::array_t<double> prices,
        const py::array_t<double> highs, const py::array_t<double>
        lows, const int periods);

py::array_t<double> cv_calc(const py::array_t<double> highs,
        const py::array_t<double> lows, const int period,
        const int smoothing_perid);

#endif
