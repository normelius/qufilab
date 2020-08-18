
#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

/*
 *  Data structure containing everything related to
 *  prices and size of numpy arrays. It requests the
 *  pointers from the buffer protocols, and exposing the
 *  data for ease of use.
 */
template <typename T>
struct Container{
    T *high;
    T *low;
    T *open;
    T *close;
    int size;

    Container(py::array_t<T> high, py::array_t<T> low, py::array_t<T> open,
        py::array_t<T> close) {

        py::buffer_info buffer = close.request();
        this -> high = (T *) high.request().ptr;
        this -> low = (T *) low.request().ptr;
        this -> open = (T *) open.request().ptr;
        this -> close = (T *) buffer.ptr;

        // Get size of the first dimension. Since we only deal with
        // 1D arrays, only one dimension exists.
        this -> size = buffer.shape[0];
    }
};


#endif 

