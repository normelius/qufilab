#ifndef MOMENTUM_H
#define MOMENTUM_H


#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> rsi_calc(const py::array_t<double> prices,
        const int periods, const std::string rsi_type);

std::tuple<py::array_t<double>, py::array_t<double>> 
    macd_calc(const py::array_t<double> prices);

py::array_t<double> willr_calc(const py::array_t<double> prices,
        const py::array_t<double> highs,
        const py::array_t<double> lows,
        const int periods);

std::tuple<std::vector<double>, std::vector<double>> 
    stochastic_calc(const std::vector<double> prices,
        const std::vector<double> highs,
        const std::vector<double> lows,
        std::string mode, const int period, 
        std::string method);

py::array_t<double> roc_calc(const py::array_t<double> prices,
        const int periods);

py::array_t<double> vpt_calc(const py::array_t<double> prices,
        const py::array_t<double> volumes);

py::array_t<double> mi_calc(const py::array_t<double> prices,
        const int periods);

py::array_t<double> cci_calc(const py::array_t<double> close,
        const py::array_t<double> high, const py::array_t<double> low,
        const int period);

py::array_t<double>
        aroon_calc(const py::array_t<double> high, 
        const py::array_t<double> low, const int period);

std::vector<double> tsi_calc(const std::vector<double> close,
        const int period, const int period_double);

py::array_t<double> apo_calc(const py::array_t<double> prices,
        const int period_slow, const int period_fast, const std::string ma);

py::array_t<double> bop_calc(const py::array_t<double> high,
    const py::array_t<double> low, const py::array_t<double> open,
    const py::array_t<double> close);

py::array_t<double> cmo_calc(const py::array_t<double> close,
        const int period);

py::array_t<double> mfi_calc(const py::array_t<double> high,
    const py::array_t<double> low, const py::array_t<double> close,
    const py::array_t<double> volume, const int period);

py::array_t<double> ppo_calc(const py::array_t<double> prices,
        const int period_fast, const int period_slow, 
        const std::string ma_type);

#endif
