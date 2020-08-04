
#ifndef INDICATOR_UTIL_H
#define INDICATOR_UTIL_H


template<typename T>
void init_nan(T (&array), const int size) {
    for (int idx = 0; idx < size; ++idx) {
        array[idx] = std::numeric_limits<double>::quiet_NaN();
    }
}

template<typename T>
void init_zeros(T (&array), const int size) {
    for (int idx = 0; idx < size; ++idx) {
        array[idx] = 0.0;
    }
}

#endif
