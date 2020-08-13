

#ifndef STAT_H
#define STAT_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<T> std_calc(const py::array_t<T> prices,
        const int period, const bool normalize);

template <typename T>
py::array_t<T> var_calc(const py::array_t<T> prices,
        const int period, const bool normalize);

template <typename T>
py::array_t<T> cov_calc(const py::array_t<T> prices,
        const py::array_t<T> market, const int period, 
        const bool normalize);

template <typename T>
py::array_t<T> beta_calc(const py::array_t<T> prices,
        const py::array_t<T> market, const int period, 
        const bool var_normalize);

template <typename T>
py::array_t<T> pct_change_calc(const py::array_t<T> prices,
        const int period);
#endif
