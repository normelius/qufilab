

#ifndef STAT_H
#define STAT_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> std_calc(const py::array_t<double> prices,
        const int period, const bool normalize);

py::array_t<double> var_calc(const py::array_t<double> prices,
        const int period, const bool normalize);

py::array_t<double> cov_calc(const py::array_t<double> prices,
        const py::array_t<double> market, const int period, 
        const bool normalize);

py::array_t<double> beta_calc(const py::array_t<double> prices,
        const py::array_t<double> market, const int period, 
        const bool var_normalize);

py::array_t<double> pct_change_calc(const py::array_t<double> prices,
        const int period);
#endif
