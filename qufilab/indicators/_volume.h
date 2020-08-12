#ifndef VOLUME_H
#define VOLUME_H

#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> acdi_calc(const py::array_t<double> price,
        const py::array_t<double> highs, 
        const py::array_t<double> lows,
        const py::array_t<double> volumes);

py::array_t<double> obv_calc(const py::array_t<double> price,
        const py::array_t<double> volumes);

py::array_t<double> cmf_calc(
        const py::array_t<double> price,
        const py::array_t<double> highs, 
        const py::array_t<double> lows,
        const py::array_t<double> volumes,
        const int periods);

py::array_t<double> ci_calc(
        const py::array_t<double> price,
        const py::array_t<double> highs, 
        const py::array_t<double> lows,
        const py::array_t<double> volumes);

py::array_t<double> pvi_calc(
        const py::array_t<double> price,
        const py::array_t<double> volumes);

py::array_t<double> nvi_calc(
        const py::array_t<double> price,
        const py::array_t<double> volumes);
#endif
