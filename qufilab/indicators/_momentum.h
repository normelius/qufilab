#ifndef MOMENTUM_H
#define MOMENTUM_H


#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<T> rsi_calc(const py::array_t<T> prices,
        const int periods, const std::string rsi_type);

template <typename T>
std::tuple<py::array_t<T>, py::array_t<T>> 
    macd_calc(const py::array_t<T> prices);

template <typename T>
py::array_t<T> willr_calc(const py::array_t<T> prices,
        const py::array_t<T> highs,
        const py::array_t<T> lows,
        const int periods);

template <typename T>
std::tuple<std::vector<T>, std::vector<T>> 
    stochastic_calc(const std::vector<T> prices,
        const std::vector<T> highs,
        const std::vector<T> lows,
        std::string mode, const int period, 
        std::string method);

template <typename T>
py::array_t<T> roc_calc(const py::array_t<T> prices,
        const int periods);

template <typename T>
py::array_t<T> vpt_calc(const py::array_t<T> prices,
        const py::array_t<T> volumes);

template <typename T>
py::array_t<T> mi_calc(const py::array_t<T> prices,
        const int periods);

template <typename T>
py::array_t<T> cci_calc(const py::array_t<T> close,
        const py::array_t<T> high, const py::array_t<T> low,
        const int period);

template <typename T>
py::array_t<T>
        aroon_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const int period);

template <typename T>
std::vector<T> tsi_calc(const std::vector<T> close,
        const int period, const int period_double);

template <typename T>
py::array_t<T> apo_calc(const py::array_t<T> prices,
        const int period_slow, const int period_fast, const std::string ma);

template <typename T>
py::array_t<T> bop_calc(const py::array_t<T> high,
    const py::array_t<T> low, const py::array_t<T> open,
    const py::array_t<T> close);

template <typename T>
py::array_t<T> cmo_calc(const py::array_t<T> close,
        const int period);

template <typename T>
py::array_t<T> mfi_calc(const py::array_t<T> high,
    const py::array_t<T> low, const py::array_t<T> close,
    const py::array_t<T> volume, const int period);

template <typename T>
py::array_t<T> ppo_calc(const py::array_t<T> prices,
        const int period_fast, const int period_slow, 
        const std::string ma_type);

#endif
