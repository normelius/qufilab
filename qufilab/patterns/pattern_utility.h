#ifndef PATTERN_UTIL_H
#define PATTERN_UTIL_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "../indicators/util.h"
#include "../indicators/_trend.h"


/*
 *  Utility function to get the averages of candlestick
 *  body sizes.
 */
template <typename T>
py::array_t<T> calc_body_avg(const py::array_t<T> close,
        const py::array_t<T> open, const int period) {

    py::buffer_info close_buf = close.request();
    const int size = close_buf.shape[0];

    auto *close_ptr = (T *) close_buf.ptr;
    auto *open_ptr = (T *) open.request().ptr;
 
    auto bodies = py::array_t<T>(close_buf.size);
    auto *bodies_ptr = (T *) bodies.request().ptr;
    init_nan(bodies_ptr, size);

    for (int idx = 0; idx < size; ++idx) {
        bodies_ptr[idx] = abs(close_ptr[idx] - open_ptr[idx]);
    }

    py::array_t<T> body_avg = ema_calc(bodies, period);

    return body_avg;
 }

#endif
