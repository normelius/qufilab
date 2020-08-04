/*
 * @ Quant, Anton Normelius, 2020.
 *
 * Module to handle indicator calculations.
 *
 * */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <limits>
#include <numeric>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "trend.h"
#include "util.h" // Init nans.

namespace py = pybind11;

/*
    SMA (Simple Moving Average)

    @param prices (py::array_t<double>): Array with prices.
    @param periods (int): Number of periods.
 */
py::array_t<double> sma_calc(const py::array_t<double> price, const int period) {
    py::buffer_info price_buf = price.request();
    double *price_ptr = (double *) price_buf.ptr;
    const int size = price_buf.shape[0];

    py::array_t<double> sma = py::array_t<double>(price_buf.size);
    double *sma_ptr = (double *) sma.request().ptr;
    init_nan(sma_ptr, size);

    // Check leading NaNs and adjust calculation below. This is needed if arg prices contain
    // leading NaNs, which will occur when calculating other indicators.
    int adjust_nan = 0;
    for (int idx = 0; idx < size; ++idx) {
        if (std::isnan(price_ptr[idx])) {
            ++adjust_nan;
        }

        else {
            break;
        }
    }

    double temp = 0;
    for (int idx = 0 + adjust_nan; idx < size; ++idx) {
        temp += price_ptr[idx]; 

        if (idx >= period + adjust_nan) {
            temp -= price_ptr[idx - period];
        }   

        if (idx >= (period - 1 + adjust_nan)) {
            sma_ptr[idx] = ((double) temp / period);
        }
    }

    return sma;
}


/*
    EMA
    Exponential Moving Average
    Math: (close - ema(prev)) * k + ema(prev)

    @param prices (vector<double>): Vector with prices.
    @param periods (int): Number of periods.
 */
py::array_t<double> ema_calc(const py::array_t<double> prices, const int periods) {
    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];
    
    py::array_t<double> ema = py::array_t<double>(prices_buf.size);
    double *ema_ptr = (double *) ema.request().ptr;
    init_nan(ema_ptr, size);
    
    // Check leading NaNs and adjust calculation below. This is needed if arg prices contain
    // leading NaNs, which will occur when calculating other indicators.
    int adjust_nan = 0;
    for (int idx = 0; idx < size; ++idx) {
        if (std::isnan(prices_ptr[idx])) {
            ++adjust_nan;
        }

        else {
            break;
        }
    }

    // Start with sma for first data point.
    double prev = std::accumulate(prices_ptr + adjust_nan, prices_ptr + periods + adjust_nan, 0.0);
    prev /= periods;

    // Multiplier, i.e. 18.18% weight with period 10;
    double k = (double) 2 / (periods + 1);
    //prev = (prices[periods-1] - prev) * k + prev;
    ema_ptr[periods - 1 + adjust_nan] = prev;

    for (int idx = periods + adjust_nan; idx < size; idx++) {
        prev = (prices_ptr[idx] - prev) * k + prev;
        ema_ptr[idx] = prev;
    }

    return ema;
}


/*
    DEMA
    Double Exponential Moving Average
    Math: DEMA = 2 * EMA_N - EMA(EMA_N).

    @param prices (py::array_t<double>): Array with prices.
    @param periods (int): Number of periods.
 */
py::array_t<double> dema_calc(const py::array_t<double> prices, const int periods) {
    
    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];

    py::array_t<double> ema1 = ema_calc(prices, periods);
    py::array_t<double> ema2 = ema_calc(ema1, periods);
    
    double *ema1_ptr = (double *) ema1.request().ptr;
    double *ema2_ptr = (double *) ema2.request().ptr;

    py::array_t<double> dema = py::array_t<double>(prices_buf.size);
    double *dema_ptr = (double *) dema.request().ptr;
    init_nan(dema_ptr, size);

    for (int idx = 2*periods-2; idx < ema1.size(); ++idx) {
        dema_ptr[idx] = 2 * ema1_ptr[idx] - ema2_ptr[idx];
    }

    return dema;
}

/*
    TEMA
    Triple Exponential Moving Average
    Math: TEMA = (3* EMA_1) - (3 * EMA_2) + EMA_3.

    @param prices (py::array_t<double>): Array with prices.
    @param periods (int): Number of periods.
 */
py::array_t<double> tema_calc(const py::array_t<double> prices, const int periods) {
    
    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];

    py::array_t<double> ema1 = ema_calc(prices, periods);
    py::array_t<double> ema2 = ema_calc(ema1, periods);
    py::array_t<double> ema3 = ema_calc(ema2, periods);
    
    double *ema1_ptr = (double *) ema1.request().ptr;
    double *ema2_ptr = (double *) ema2.request().ptr;
    double *ema3_ptr = (double *) ema3.request().ptr;

    py::array_t<double> tema = py::array_t<double>(prices_buf.size);
    double *tema_ptr = (double *) tema.request().ptr;
    init_nan(tema_ptr, size);

    for (int idx = 3*periods-3; idx < prices.size(); ++idx) {
        tema_ptr[idx] = (3*ema1_ptr[idx]) - (3*ema2_ptr[idx]) + ema3_ptr[idx];
    }

    return tema;
}

/*
    T3
    T3 Moving Average.

    Math: T3 = c1*e6 + c2*e5 + c3*e4 + c4*e3.
        e1 = EMA (Close, Period)
        e2 = EMA (e1, Period)
        e3 = EMA (e2, Period)
        e4 = EMA (e3, Period)
        e5 = EMA (e4, Period)
        e6 = EMA (e5, Period)
        a is the volume factor, default value is 0.7 but 0.618 can also be used.
        c1 = – a^3
        c2 = 3*a^2 + 3*a^3
        c3 = – 6*a^2 – 3*a – 3*a^3
        c4 = 1 + 3*a + a^3 + 3*a^2

    @param prices (py::array_t<double>): Array with prices.
    @param periods (int): Number of periods.
 */
py::array_t<double> t3_calc(const py::array_t<double> prices, const int periods,
        const double volume_factor) {
    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];

    py::array_t<double> t3 = py::array_t<double>(prices_buf.size);
    double *t3_ptr = (double *) t3.request().ptr;
    init_nan(t3_ptr, size);
    
    py::array_t<double> ema1 = ema_calc(prices, periods);
    py::array_t<double> ema2 = ema_calc(ema1, periods);
    py::array_t<double> ema3 = ema_calc(ema2, periods);
    py::array_t<double> ema4 = ema_calc(ema3, periods);
    py::array_t<double> ema5 = ema_calc(ema4, periods);
    py::array_t<double> ema6 = ema_calc(ema5, periods);

    double *ema3_ptr = (double *) ema3.request().ptr;
    double *ema4_ptr = (double *) ema4.request().ptr;
    double *ema5_ptr = (double *) ema5.request().ptr;
    double *ema6_ptr = (double *) ema6.request().ptr;
    
    double c1 = -std::pow(volume_factor, 3);
    double c2 = 3 * std::pow(volume_factor, 2) + 3 * std::pow(volume_factor, 3);
    double c3 = - 6 * std::pow(volume_factor, 2) - 3 * volume_factor - 3 * std::pow(volume_factor, 3);
    double c4 = 1 + 3 * volume_factor + std::pow(volume_factor, 3) + 3 * std::pow(volume_factor, 2);
    
    for (int idx = periods*5-1; idx < size; ++idx) {
        t3_ptr[idx] = c1*ema6_ptr[idx] + c2*ema5_ptr[idx] + c3*ema4_ptr[idx] + c4*ema3_ptr[idx];
    }

    return t3;
}


/*
    Triangular Moving Average
    Math: 
        If period is even: first_period = period / 2.
                           second_period = (period / 2) + 1.
        If period is uneven: first_period = second_period = (period+1)/2 ronded up.

        TMA = SMA(SMA(price, first_period), second_period)
    
    @param prices (py::array_t<double>): Array with prices.
    @param periods (int): Number of periods.

    OBSERVE that this implementation uses two sma calculations and can be optimzied
    further by extending the original sma implementation. However, it is going to 
    take some time doing the math so, leaving this for a rainy day.
 */
py::array_t<double> tma_calc(const py::array_t<double> prices, const int period) {
    
    int first_period;
    int second_period;

    if (period % 2 == 0) {
        first_period = period / 2;
        second_period = (period / 2) + 1;
    }

    else {
        first_period = std::ceil((double) (period+1)/2);
        second_period = std::ceil((double) (period+1)/2);
    }

    py::array_t<double> sma = sma_calc(prices, first_period);
    py::array_t<double> tma = sma_calc(sma, second_period);

    return tma;
}

/*
    SMMA
    Smoothed Moving Average
    Math: 
        1. First value = sma.
        2. SMMA(i) = (SMMA1(i - 1) * (periods - 1) + prices(i)) / periods.
    
    @param prices (vector<double>): Vector with prices.
    @param periods (int): Number of periods.
    @param defualt_size (int): Specify whether the returned vector
        should be the default size with NaNs or not.
 */
py::array_t<double> smma_calc(const py::array_t<double> prices, const int periods) {

    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];
    
    py::array_t<double> smma = py::array_t<double>(prices_buf.size);
    double *smma_ptr = (double *) smma.request().ptr;
    init_nan(smma_ptr, size);

    // Start with sma for first data point.
    double prev = std::accumulate(prices_ptr, prices_ptr + periods, 0.0);
    prev /= periods;
    //double prev = (smma1 * (periods - 1) + prices[periods-1]) / periods;
    smma_ptr[periods-1] = prev;

    for (int idx = periods; idx < size; idx++) {
        prev = (prev * (periods - 1) + prices_ptr[idx]) / periods;
        smma_ptr[idx] = prev;
    }

    return smma;
}


/*
   LWMA
   Linear Weighted Moving Average
Math: LWMA = sum(prices[i] * W(i)) / sum(W),
where W are the weights, ranging from 1-periods.

@param prices (vector<double>): Vector with prices.
@param periods (int): Number of periods.
@param defualt_size (int): Specify whether the returned vector
should be the default size with NaNs or not.
*/
py::array_t<double> lwma_calc(const py::array_t<double> prices, const int periods) {

    py::buffer_info prices_buf = prices.request();
    double *prices_ptr = (double *) prices_buf.ptr;
    const int size = prices_buf.shape[0];

    py::array_t<double> lwma = py::array_t<double>(prices_buf.size);
    double *lwma_ptr = (double *) lwma.request().ptr;
    init_nan(lwma_ptr, size);

    for (int ii = 0; ii < size - periods + 1; ii++) {
        double temp = 0;
        int W = 1;
        int W_sum = 0;
        for (int idx = ii; idx < periods + ii; idx++) {
            temp += (prices_ptr[idx] * W);
            W_sum += W;
            W++;
        }
        temp /= W_sum;
        lwma_ptr[ii+periods-1] = temp;
    }

    return lwma;
}

/*
    WC
    Weighted Close
    Math: wc[i] = ((close * 2) + high + low) / 4,

    @param prices (vector<double>): Vector with closing prices.
    @param highs (vector<double>): Vector with high prices.
    @param lows (vector<double>): Vector with low prices.
*/
py::array_t<double> wc_calc(const py::array_t<double> closes, const py::array_t<double> highs,
        const py::array_t<double> lows) {

    py::buffer_info closes_buf = closes.request();
    double *closes_ptr = (double *) closes_buf.ptr;
    py::buffer_info highs_buf = highs.request();
    double *highs_ptr = (double *) highs_buf.ptr;
    py::buffer_info lows_buf = lows.request();
    double *lows_ptr = (double *) lows_buf.ptr;
    
    const int size = closes_buf.shape[0];
    py::array_t<double> wc = py::array_t<double>(closes_buf.size);
    py::buffer_info wc_buf = wc.request();
    double *wc_ptr = (double *) wc_buf.ptr;
    // Initialize NaNs
    init_nan(wc_ptr, size);

    for (int idx = 0; idx < size; ++idx) {
        wc_ptr[idx] = ((closes_ptr[idx] * 2) + highs_ptr[idx] + lows_ptr[idx]) / 4;
    }

    return wc;
}



PYBIND11_MODULE(trend, m) {
    m.def("sma_calc", &sma_calc, "Simple Moving Average");
    m.def("ema_calc", &ema_calc, "Exponential Moving Average");
    m.def("dema_calc", &dema_calc, "Double Exponential Moving Average");
    m.def("tema_calc", &tema_calc, "Triple Exponential Moving Average");
    m.def("t3_calc", &t3_calc, "T3 Moving Average");
    m.def("tma_calc", &tma_calc, "Triangular Moving Average");
    m.def("smma_calc", &smma_calc, "Smoothed Moving Average");
    m.def("lwma_calc", &lwma_calc, "Linear Weighted Moving Average");
    m.def("wc_calc", &wc_calc, "Weighted Close");
}
