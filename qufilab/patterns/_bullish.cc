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
        // Specific values on the body of a candlestick.
        T body_max = std::max(close_ptr[idx], open_ptr[idx]);
        T body_min = std::min(close_ptr[idx], open_ptr[idx]);

        // Actual distances.
        T body = abs(close_ptr[idx] - open_ptr[idx]);
        T upper_shadow = abs(high_ptr[idx] - body_max);
        T lower_shadow = abs(body_min - low_ptr[idx]);
           
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

/*
 * Implementation of DRAGONFLY_DOJI.
 * 
 * Definition:
 *  Small body, i.e. open and close near eachother.
 *  Opens and close near the high.
 *  Long lower shadow.
 *  
 * Params:
 *  open (py::array_t<T>) : Array with opening prices.
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  close (py::array_t<T>) : Array with close prices.
 *  len_bottom_shadow (T) : Specify how long lower shadow should be
 *      compared to the bottom. Default 10x the body length.
 *
 */
template <typename T>
py::array_t<bool> dragonfly_doji_calc(py::array_t<T> open, py::array_t<T> high, 
        py::array_t<T> low, py::array_t<T> close, const T len_bottom_shadow) {
    
    py::buffer_info close_buf = close.request();
    const int size = close_buf.shape[0];

    // Get pointers to the data.
    auto *close_ptr = (T *) close_buf.ptr;
    auto *open_ptr = (T *) open.request().ptr;
    auto *high_ptr = (T *) high.request().ptr;
    auto *low_ptr = (T *) low.request().ptr;

    // Create array for dragonfly doji values based on input argument size
    // and initialize a zero array.
    auto dfd = py::array_t<bool>(close_buf.size);
    auto *dfd_ptr = (bool *) dfd.request().ptr;
    init_false(dfd_ptr, size);
    
    for (int idx = 0; idx < size; ++idx) {
        // Specific values on the body of a candlestick.
        T body_max = std::max(close_ptr[idx], open_ptr[idx]);
        T body_min = std::min(close_ptr[idx], open_ptr[idx]);

        // Actual distances.
        T body = abs(close_ptr[idx] - open_ptr[idx]);
        T upper_shadow = abs(high_ptr[idx] - body_max);
        T lower_shadow = abs(body_min - low_ptr[idx]);
        
        // Upper shadow significantly shorter than the body,
        // lower part of body close to high and long lower shadow.
        // Close to high is here implemented that the lower part of body 
        // is bigger than 99.8% of high, i.e. it is very close to the high.
        // Of course this is open to interpretation "in mathematical terms, 
        // how close should the lower body be to the high".
        if ((upper_shadow < body * 0.1) && (body_min >= high_ptr[idx] * 0.998) && 
                (lower_shadow >= body * len_bottom_shadow)) {
            dfd_ptr[idx] = true;
        }
    }    

    return dfd;
}


PYBIND11_MODULE(_bullish, m) {
    m.def("hammer_calc", &hammer_calc<double>, "Hammer pattern");
    m.def("hammer_calc", &hammer_calc<float>, "Hammer pattern");

    m.def("dragonfly_doji_calc", &dragonfly_doji_calc<double>, "Dragonfly doji pattern");
    m.def("dragonfly_doji_calc", &dragonfly_doji_calc<float>, "Dragonfly doji pattern");
}
