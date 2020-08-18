#ifndef BULLISH_H
#define BULLISH_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template <typename T>
py::array_t<bool> hammer_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period, const std::string hammer_type,
        const T shadow_margin);

template <typename T>
py::array_t<bool> dragonfly_doji_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period);

template <typename T>
py::array_t<bool> doji_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period);

template <typename T>
py::array_t<bool> marubozu_white_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, T shadow_margin, const int period);

template <typename T>
py::array_t<bool> spinning_top_white_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period);

template <typename T>
py::array_t<bool> engulfing_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int trend_period, 
        const std::string engulfing_type);

template <typename T>
py::array_t<bool> harami_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int trend_period, 
        const std::string harami_type);

template <typename T>
py::array_t<bool> kicking_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int trend_period, 
        const std::string kicking_type, const float shadow_margin);

#endif
