/*
 *  @QufiLab, Anton Normelius, 2020.
 *
 *  Bullish candlestick patterns.
 *
 */

#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <numeric>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "../indicators/util.h"

namespace py = pybind11;

/*
 * Implementation of HAMMER.
 *
 *
 */
template <typename T>
py::array_t<bool> hammer_calc(py::array_t<T> open, py::array_t<T> high, 
        py::array_t<T> low, py::array_t<T> close) {
    
    py::buffer_info close_buf = close.request();
    const int size = close_buf.shape[0];

    // Get pointers to the data.
    auto *close_ptr = (T *) close_buf.ptr;
    auto *open_ptr = (T *) open.request().ptr;
    auto *high_ptr = (T *) high.request().ptr;
    auto *low_ptr = (T *) low.request().ptr;

    // Create array for hammer values based on input argument size
    // and initialize a zero array.
    auto hammer = py::array_t<bool>(close_buf.size);
    auto *hammer_ptr = (bool *) hammer.request().ptr;
    init_false(hammer_ptr, size);
    
    // Upper shadow is less than 10% of body, indicating
    // a very small/close to none upper shadow.
    float upper_limit = 0.10;

    for (int idx = 0; idx < size; ++idx) {
        T body = abs(open_ptr[idx] - close_ptr[idx]);
        T max = std::max(close_ptr[idx], open_ptr[idx]);
        T min = std::min(close_ptr[idx], open_ptr[idx]);
        T upper_shadow = abs(high_ptr[idx] - max);
        T lower_shadow = abs(min - low_ptr[idx]);
           
        // Check that no upper_shadow exist and that the lower shadow
        // is less than 3x the body but larger than 2x the body.
        if (upper_shadow <= (body * upper_limit) &&
            lower_shadow >= (body * 2) &&
            lower_shadow <= (body * 3)) {
            hammer_ptr[idx] = true;
        }
    }

    return hammer;
}




PYBIND11_MODULE(_bullish, m) {
    m.def("hammer_calc", &hammer_calc<double>, "Hammer pattern");
    m.def("hammer_calc", &hammer_calc<float>, "Hammer pattern");
}
