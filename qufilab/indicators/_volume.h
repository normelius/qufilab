#ifndef VOLUME_H
#define VOLUME_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<T> acdi_calc(const py::array_t<T> price,
        const py::array_t<T> highs, 
        const py::array_t<T> lows,
        const py::array_t<T> volumes);

template <typename T>
py::array_t<T> obv_calc(const py::array_t<T> price,
        const py::array_t<T> volumes);

template <typename T>
py::array_t<T> cmf_calc(
        const py::array_t<T> price,
        const py::array_t<T> highs, 
        const py::array_t<T> lows,
        const py::array_t<T> volumes,
        const int periods);

template <typename T>
py::array_t<T> ci_calc(
        const py::array_t<T> price,
        const py::array_t<T> highs, 
        const py::array_t<T> lows,
        const py::array_t<T> volumes);

template <typename T>
py::array_t<T> pvi_calc(
        const py::array_t<T> price,
        const py::array_t<T> volumes);

template <typename T>
py::array_t<T> nvi_calc(
        const py::array_t<T> price,
        const py::array_t<T> volumes);

#endif
