/*
    @Quant. 
    Anton Normelius. 2020.

    Volatility indicators.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>  
#include <chrono>
#include <limits>
#include <omp.h>
#include <x86intrin.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "_volatility.h"
#include "_trend.h"
#include "_stat.h"
#include "util.h"

namespace py = pybind11;


/*
    Calculation of Standard Deviation using normalization
    with N - 1, where N is the sample size, i.e. number of periods.
 */
/*
py::array_t<double> std_calc(const py::array_t<double> prices, 
        const int periods, const bool normalize) {

    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];

    py::array_t<double> sma = sma_calc(prices, periods);    
    double *sma_ptr = (double *) sma.request().ptr;

    py::array_t<double> stdvec = py::array_t<double>(prices_buf.size);
    double *stdvec_ptr = (double *) stdvec.request().ptr;
    init_nan(stdvec_ptr, size);

    for (int ii = 0; ii < size - periods + 1; ii++) {
        double temp = 0;

        for (int idx = ii; idx < periods + ii; idx++) {
            temp += pow((prices_ptr[idx] - sma_ptr[ii+periods-1]), 2);
        }

        if (normalize == true) {
            temp /= (periods - 1);
        }

        else {
            temp /= (periods);
        }

        temp = sqrt(temp);
        stdvec_ptr[ii+periods-1] = temp;
    }
    
    return stdvec;
}
*/

/*
    Calculates Bollinger Bands.
    Math: middle = SMA(periods).
          top = middle + (std_dev * std)
          bottom = middle - (std_dev * std)
    
    @param prices (vector<double>): Vector with prices.
    @param periods (int): Number of periods.
    @param deviation (int): Number of deviations from the mean.
        Multiplied with standard deviation.

    @return (tuple): Lower and upper bollinger bands.
 */
std::tuple<py::array_t<double>, py::array_t<double>, py::array_t<double>> 
    bbands_calc(const py::array_t<double> prices, 
        const int periods, const int deviation) {
   
    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];

    py::array_t<double> sma = sma_calc(prices, periods);
    const double *sma_ptr = (double *) sma.request().ptr;

    // Observe no normalization of the standard deviation.
    py::array_t<double> std = std_calc(prices, periods, false);
    const double *std_ptr = (double *) std.request().ptr;

    py::array_t<double> middle = py::array_t<double>(prices_buf.size);
    py::array_t<double> lower = py::array_t<double>(prices_buf.size);
    py::array_t<double> upper = py::array_t<double>(prices_buf.size);
    double *middle_ptr = (double *) middle.request().ptr;
    double *lower_ptr = (double *) lower.request().ptr;
    double *upper_ptr = (double *) upper.request().ptr;

    init_nan(middle_ptr, size);
    init_nan(lower_ptr, size);
    init_nan(upper_ptr, size);
    
    for (int idx = periods-1; idx < size; idx++) {
        middle_ptr[idx] = sma_ptr[idx];
        lower_ptr[idx] = sma_ptr[idx] - (deviation * std_ptr[idx]);
        upper_ptr[idx] = sma_ptr[idx] + (deviation * std_ptr[idx]);
    }

    return std::make_tuple(upper, middle, lower);
}

/*
    Keltner Channels.
    Math: middle = EMA(periods).
          top = middle + (deviation * ATR(periods_atr))
          bottom = middle - (deviation * ATR(periods_atr))
    
    @param prices (vector<double>): Vector with prices.
    @param periods (int): Number of periods.
    @param periods_atr (int): Number of periods for the atr calculations.
    @param deviation (int): Number of deviations from ema.
        Multiplied with atr.
 */
std::tuple<py::array_t<double>, py::array_t<double>, py::array_t<double>>
    kc_calc(const py::array_t<double> prices, const py::array_t<double> highs, 
            const py::array_t<double> lows, const int period, const int period_atr, const int deviation) {

        py::buffer_info prices_buf = prices.request();
        const int size = prices_buf.shape[0];
        
        py::array_t<double> lower = py::array_t<double>(prices_buf.size);
        py::array_t<double> upper = py::array_t<double>(prices_buf.size);
        double *lower_ptr = (double *) lower.request().ptr;
        double *upper_ptr = (double *) upper.request().ptr;
    
        init_nan(lower_ptr, size);
        init_nan(upper_ptr, size);

        py::array_t<double> middle = ema_calc(prices, period);
        py::array_t<double> atr = atr_calc(prices, highs, lows, period_atr);
        double *middle_ptr = (double *) middle.request().ptr;
        double *atr_ptr = (double *) atr.request().ptr;
        
        // If period_atr is greater than period, subtraction with NaN values
        // will happen, however it is fine for now since it will also result in NaNs.
        for (int idx = period; idx < size; ++idx) {
            lower_ptr[idx] = middle_ptr[idx] - (deviation * atr_ptr[idx]);
            upper_ptr[idx] = middle_ptr[idx] + (deviation * atr_ptr[idx]);
        }
        middle_ptr[period-1] = std::numeric_limits<double>::quiet_NaN();
        return std::make_tuple(upper, middle, lower);
}   

/*
    Average True Range (ATR).
    Consists of taking the exponential average (standard 14 days) of the True Range.
    True Range vector will have one day's NaN since starting value should
    compare to yesterday's closing price.

    Math: True range-value is the greatest of the following.
        1. Today's high minus todays's low.
        2. Absolute value of today's high minus yesterday's close.
        3. Absolute value of today's low minus yesterday's close.

    Observe that there exist 10 unstable periods when calculating using 10 periods,
    since two of the conditions below includes yesterday prices, meaning the first ATR
    can't be calcualted (since it doesn't exist any values before the first), and hence
    one more NaN is included in the returned array.
 */
py::array_t<double> atr_calc(const py::array_t<double> prices, 
        const py::array_t<double> highs, const py::array_t<double> lows, 
        const int periods) {

    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];

    double *highs_ptr = (double *) highs.request().ptr;
    double *lows_ptr = (double *) lows.request().ptr;
    
    py::array_t<double> tr = py::array_t<double>(prices_buf.size);
    py::array_t<double> atr = py::array_t<double>(prices_buf.size);
    double *tr_ptr = (double *) tr.request().ptr;
    double *atr_ptr = (double *) atr.request().ptr;
    
    init_nan(tr_ptr, size);
    init_nan(atr_ptr, size);

    for (int idx = 1; idx < size; ++idx) {
        double condition1 = highs_ptr[idx] - lows_ptr[idx];
        double condition2 = abs(highs_ptr[idx] - prices_ptr[idx-1]);
        double condition3 = abs(lows_ptr[idx] - prices_ptr[idx-1]);
        tr_ptr[idx] = std::max({condition1, condition2, condition3});

        if (idx == periods) {
            // First ATR-value is a simple mean from the TR-values.
            atr_ptr[idx] = std::accumulate(tr_ptr + 1, tr_ptr + periods + 1, 0.0) / periods;
        }
        
        // Subsequent ATR-values uses a smoothing average of the TR-values
        if (idx > periods) {
            atr_ptr[idx] = (atr_ptr[idx-1] * (periods - 1) + tr_ptr[idx]) / periods;
        }
    }

    return atr;
}

/*
    Chaikin Volatility (CV).
    Math: True range-value is the greatest of the following.

    @param highs (vector<double>): Vector with high prices.
    @param lwos (vector<double>): Vector with low prices.
    @param period (int): Number of periods.
    @param smoothing_period (int): Number of periods for the smoothing of the ema.

 */

py::array_t<double> cv_calc(const py::array_t<double> highs,
   const py::array_t<double> lows, const int period, const int smoothing_period) {

   //std::vector<double> diff(highs.size());
   //std::vector<double> cv(highs.size(), std::numeric_limits<double>::quiet_NaN());
    
    py::buffer_info highs_buf = highs.request();
    const int size = highs_buf.shape[0];
    double *highs_ptr = (double *) highs_buf.ptr;
    double *lows_ptr = (double *) lows.request().ptr;
    
    py::array_t<double> diff = py::array_t<double>(highs_buf.size);
    double *diff_ptr = (double *) diff.request().ptr;

    // Get difference between high and lows.
    std::transform(highs_ptr, highs_ptr+size, lows_ptr, diff_ptr,
    std::minus<double>());

    // Calculate the ema for the differences.
    py::array_t<double> ema = ema_calc(diff, period);
    double *ema_ptr = (double *) ema.request().ptr;
        
    py::array_t<double> cv = py::array_t<double>(highs_buf.size);
    double *cv_ptr = (double *) cv.request().ptr;
    init_nan(cv_ptr, size);

    for (int idx = period + smoothing_period - 2; idx < size; ++idx) {
        cv_ptr[idx] = ((ema_ptr[idx] - ema_ptr[idx - smoothing_period + 1]) / (ema_ptr[idx - smoothing_period + 1])) * 100;
    }
    
    return cv;
}



// TEST SSE OPTIMIZATION
/*
std::vector<double> sse_calc(std::vector<double> prices, std::vector<double> highs,
        std::vector<double> lows) {

    prices.insert(prices.begin(), 0.0);
    prices.erase(prices.end() - 1);
    int periods = 14;
    std::vector<double> tr(prices.size());

    // Used for taking absolute value.
    __m128 abs = _mm_set_ps1(-0.0);
    for (int i = 0; i < prices.size(); i += 2) {

        __m128 price = _mm_load_pd(&prices[i]); 
        __m128 high = _mm_load_pd(&highs[i]); 
        __m128 low = _mm_load_pd(&lows[i]); 

        __m128 sub_high_low = _mm_sub_pd(high, low);
        __m128 sub_high_price = _mm_andnot_pd(abs, _mm_sub_pd(high, price));
        __m128 sub_low_price = _mm_andnot_pd(abs, _mm_sub_pd(low, price));
        __m128 first_max = _mm_max_pd(sub_high_low, sub_high_price);
        __m128 last_max = _mm_max_pd(first_max, sub_low_price);
        _mm_store_pd(&tr[i], last_max);
    }

    tr[0] = highs[0] - lows[0];
    return ema_calc(tr, periods, true);
}

*/


PYBIND11_MODULE(_volatility, m) {

    m.def("std_calc", &std_calc, "Standard deviation calculations");
    m.def("bbands_calc", &bbands_calc, "Bollinger bands calculations");
    m.def("kc_calc", &kc_calc, "Keltner Channels");
    m.def("atr_calc", &atr_calc, "Average True Range calculations");
    m.def("cv_calc", &cv_calc, "Chaikin Volatility");
    //m.def("sse_calc", &sse_calc, "");

}
