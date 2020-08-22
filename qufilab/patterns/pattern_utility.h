#ifndef PATTERN_UTIL_H
#define PATTERN_UTIL_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "../indicators/util.h"
#include "../indicators/_trend.h"


/*
 *  Utility function to get the averages of candlestick
 *  body sizes.
 */
template <typename T>
T* get_body_avg(const py::array_t<T> close,
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
    T *body_avg_ptr = (T *) body_avg.request().ptr;
    return body_avg_ptr;
 }

/*
 * Utility function to get trend using simple moving average.
 */
template <typename T>
T* get_trend(std::string type, const py::array_t<T> close,
        const int trend_period) {
    py::array_t<T> ma = sma_calc(close, trend_period);
    return (T *) ma.request().ptr;
}


#endif
