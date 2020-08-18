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

#include "_bullish.h"

#include "candlestick.h"
#include "data_container.h"
#include "pattern_utility.h"

#include "../indicators/util.h"
#include "../indicators/_trend.h"

namespace py = pybind11;

/*
 * Implementation of HAMMER.
 *
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *
 * Definition for this implementation:
 *  Hammer definition
 *      - Small body, i.e. candlestick body size is smaller than 
 *          the 14 period ema body size. See _pattern_util.h for implementation.
 *          Observer that the body cannot be a doji body, since it then will be
 *          a dragonfly doji.
 *      - No/short upper shadow, i.e. upper shadow less than 5% of the body.
 *          See _pattern_util.h for implementation.
 *      - Lower shadow bigger than 2x the body size. Observe that some references
 *          specify that the lower shadow should be between 2-3 times the body.
 *
 *  Inverted Hammer definition
 *      - Small body, i.e. candlestick body size is smaller than 
 *          the 14 period ema body size. See _pattern_util.h for implementation.
 *          Observer that the body cannot be a doji body, since it then will be
 *          a gravestone doji.
 *      - No/short lower shadow, i.e. lower shadow less than 5% of the body.
 *          See _pattern_util.h for implementation.
 *      - Upper shadow bigger than 2x the body size. Observe that some references
 *          specify that the upper shadow should be between 2-3 times the body.
 */
template <typename T>
py::array_t<bool> hammer_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period, const std::string hammer_type,
        const T shadow_margin) {
    
    Container<T> data = {high, low, open, close};

    auto hammer = py::array_t<bool>(data.size);
    auto *hammer_ptr = (bool *) hammer.request().ptr;
    init_false(hammer_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    py::array_t<T> body_avg = calc_body_avg(open, close, 14);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma = sma_calc(close, period);
    auto *ma_ptr = (T *) ma.request().ptr;

    for (int idx = 14; idx < data.size; ++idx) {
        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};

        // Calculations for standard hammer.
        if (hammer_type == "hammer" && candle.has_short_body() && 
                !candle.has_doji_body() && !candle.has_upper_shadow(shadow_margin) &&
                candle.lower_shadow >= candle.body * 2) {
            hammer_ptr[idx] = true;
        }
        
        // Calculations for the inverted hammer.
        else if (hammer_type == "inverted_hammer" && candle.has_short_body() && 
                !candle.has_doji_body() && !candle.has_lower_shadow(shadow_margin) &&
                candle.upper_shadow >= candle.body * 2) {
            hammer_ptr[idx] = true;
        }
    }

    return hammer;
}


/*
 * Implementation of DOJI.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *
 * Definition:
 *  - Candlestick has a doji body, i.e. implemented here as a body 
 *      smaller than 5% of the whole candlestick range. See implementation
 *      in _pattern_util.h for more info.
 *  - Upper shadow and lower shadow are basically equal. This is handled
 *  by has_equal_shadows method in _pattern_util. Some margins are 
 *  required to catch more doji patterns. 
 */
template <typename T>
py::array_t<bool> doji_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period) {

    Container<T> data = {high, low, open, close};

    auto doji = py::array_t<bool>(data.size);
    auto *doji_ptr = (bool *) doji.request().ptr;
    init_false(doji_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    py::array_t<T> body_avg = calc_body_avg(open, close, 14);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma = sma_calc(close, period);
    auto *ma_ptr = (T *) ma.request().ptr;

    for (int idx = 14; idx < data.size; ++idx) {
        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
        
        if (candle.has_doji_body() && candle.has_equal_shadows()) {
            doji_ptr[idx] = true;
        }

    }    

    return doji;
}


/*
 * Implementation of DRAGONFLY_DOJI.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *
 * Definition:
 *  - Candlestick has a doji body, i.e. implemented here as a body 
 *      smaller than 5% of the whole candlestick range. See implementation
 *      in _pattern_util.h for more info.
 *  - No/small upper shadow. Since body already is small in a doji,
 *      the specification here is that the shadow should be less than 
 *      the body.
 */
template <typename T>
py::array_t<bool> dragonfly_doji_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        const py::array_t<T> close, const int period) {
    
    Container<T> data = {high, low, open, close};

    // Create array for dragonfly doji values based on input argument size
    // and initialize a zero array.
    auto dfd = py::array_t<bool>(data.size);
    auto *dfd_ptr = (bool *) dfd.request().ptr;
    init_false(dfd_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    py::array_t<T> body_avg = calc_body_avg(open, close, 14);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma = sma_calc(close, period);
    auto *ma_ptr = (T *) ma.request().ptr;

    for (int idx = 14; idx < data.size; ++idx) {
        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
        
        if (candle.has_doji_body() && candle.upper_shadow <= candle.body) {
            dfd_ptr[idx] = true;
        }

    }    

    return dfd;
}

/*
 * Implementation of MARUBOZU_WHITE.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *  shadow_margin (float) : Float specifying what margin should be allowed
 *      for the shadows. For example, by using shadow_marign = 5, one allows
 *      the upper/lower shadows to be as high as 5% of the body size.
 *
 * Definition:
 *  - Candlestick body should be long. This is implemented as being bigger
 *      than the 14 period ema of the body sizes.
 *  - Candlestick should have no (or very little) upper and lower shadow.
 *      This implementation doesn't strictly enforce zero shadows, but
 *      using a small margin which can be specified with the 'shadow_margin'
 *      parameter.
 */
template <typename T>
py::array_t<bool> marubozu_white_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        py::array_t<T> close, T shadow_margin, const int period) {

    Container<T> data = {high, low, open, close};
    
    auto marubozu_white = py::array_t<bool>(data.size);
    auto *marubozu_white_ptr = (bool *) marubozu_white.request().ptr;
    init_false(marubozu_white_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    py::array_t<T> body_avg = calc_body_avg(open, close, 14);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma = sma_calc(close, period);
    auto *ma_ptr = (T *) ma.request().ptr;

    for (int idx = 14; idx < data.size; ++idx) {
        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
        
        if (candle.has_long_body() && !candle.has_upper_shadow(shadow_margin) && 
                !candle.has_lower_shadow(shadow_margin)) {
            marubozu_white_ptr[idx] = true;
        }
    }    

    return marubozu_white;
}

/*
 * Implementation of SPINNING_TOP_WHITE.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *
 * Definition:
 *  - Candlestick needs to be green, i.e. it is a positive candle.
 *  - Candlestick body can't be a doji.
 *  - Upper and lower shadows needs to be greater than the body. By calculating
 *      using the range of the candlestick, it is ensured that the shadows
 *      are quite symmetrical in size, which is what we want. Another approach
 *      to the implementation is to just check that the shadows are greater than
 *      the body size; however, this will have the effect that one shadow can be
 *      greatly bigger than another. Since this implementation assumes we want 
 *      symmetry (at least to some extent), the range is used instead.
 */
template <typename T>
py::array_t<bool> spinning_top_white_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        py::array_t<T> close, const int period) {
    
    Container<T> data = {high, low, open, close};

    auto res = py::array_t<bool>(data.size);
    auto *res_ptr = (bool *) res.request().ptr;
    init_false(res_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    py::array_t<T> body_avg = calc_body_avg(open, close, 14);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma = sma_calc(close, period);
    auto *ma_ptr = (T *) ma.request().ptr;

    for (int idx = 14; idx < data.size; ++idx) {
        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
        
        // By ensuring that the upper and lower shadows are greater than
        // 1/3 of the range, we know that both shadows will be greater
        // than the the body and that the shadows will be quite similar in size.
        if (!candle.has_doji_body() && candle.is_green() &&
                candle.upper_shadow >= candle.range * 1.0/3 && 
                candle.lower_shadow >= candle.range * 1.0/3) {

            res_ptr[idx] = true;
        }

    }    

    return res;
}

/*
 * Implementation of ENGULFING.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *  trend (string) : Specify whether a trend detection should be included
 *      in the pattern. By defualt, no trend is specified.
 *  trend_period (int) : Period for moving average in order to identify trend.
 *  engulfing_type (string) : Specify what kind of engulfing type that should
 *      be calculated. Can choose from 'bull' or 'bear'.
 *
 * Definition:
 */
template <typename T>
py::array_t<bool> engulfing_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        py::array_t<T> close, const int trend_period, const std::string engulfing_type) {
    
    Container<T> data = {high, low, open, close};

    auto res = py::array_t<bool>(data.size);
    auto *res_ptr = (bool *) res.request().ptr;
    init_false(res_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    const int body_avg_period = 14;
    py::array_t<T> body_avg = calc_body_avg(open, close, body_avg_period);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma =  sma_calc(close, trend_period);
    auto *ma_ptr = (T *) ma.request().ptr;
    
    // Check start index based on the different periods. Observe we need +1 since
    // this pattern requires two candlesticks.
    int start_idx = body_avg_period + 1;

    for (int idx = start_idx; idx < data.size; ++idx) {
        Candlestick<T> candle_prev = {data.high[idx-1], data.low[idx-1], 
            data.open[idx-1], data.close[idx-1], body_avg_ptr[idx-1], ma_ptr[idx-1]};

        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
    
        // Calculations for engulfing bull. Observe that both days body high 
        // and body low can't be the same, only one can be the same.
        if (engulfing_type == "bull" && candle.is_green() && candle.has_long_body() &&
                candle_prev.is_red() && candle_prev.has_short_body() && 
                candle.open <= candle_prev.close && candle.close >= candle_prev.open &&
                (candle.open < candle_prev.close || candle.close > candle_prev.open)) {

                    res_ptr[idx] = true;
        }

        // Calculations for engulfing bear. Observe that both days body high 
        // and body low can't be the same, only one can be the same.
        else if (engulfing_type == "bear" && candle.is_red() && candle.has_long_body() &&
                candle_prev.is_green() && candle_prev.has_short_body() && 
                candle.open >= candle_prev.close && candle.close <= candle_prev.open &&
                (candle.open > candle_prev.close || candle.close < candle_prev.open)) {

                    res_ptr[idx] = true;
        }

    }

    return res;
}

/*
 * Implementation of HARAMI.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *  trend_period (int) : Period for moving average in order to identify trend.
 *  harami_type (string) : Specify what kind of harami type that should
 *      be calculated. Can choose from 'bull' or 'bear'.
 *
 * Definition:
 * Observe that there exists numerous different interpretations of this pattern,
 * read the definition below to see how this is implemented.
 *
 *  Harami bull.
 *      - Previous candle needs to be to be a long red candle.
 *      - Current candle needs to be within the body range of the previous candle.
 *          The current body high can be the same as previous body high, or current
 *          body low can be the same as previous body low, but now both at the 
 *          same time.
 *      - In this implementation, current candle needs to be small, however NOT a doji
 *          candlestick.
 *      - Current candle color does not matter.
 *
 *  Harami bear.
 *      - Previous candle needs to be to be a long green candle.
 *      - Current candle needs to be within the body range of the previous candle.
 *          The current body high can be the same as previous body high, or current
 *          body low can be the same as previous body low, but now both at the 
 *          same time.
 *      - In this implementation, current candle needs to be small, however NOT a doji
 *          candlestick.
 *      - Current candle color does not matter.
 *
 */
template <typename T>
py::array_t<bool> harami_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        py::array_t<T> close, const int trend_period, const std::string harami_type) {
    
    Container<T> data = {high, low, open, close};
    
    auto res = py::array_t<bool>(data.size);
    auto *res_ptr = (bool *) res.request().ptr;
    init_false(res_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    const int body_avg_period = 14;
    py::array_t<T> body_avg = calc_body_avg(open, close, body_avg_period);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma =  sma_calc(close, trend_period);
    auto *ma_ptr = (T *) ma.request().ptr;
    
    // Check start index based on the different periods. Observe we need +1 since
    // this pattern requires two candlesticks.
    int start_idx = body_avg_period + 1;

    for (int idx = start_idx; idx < data.size; ++idx) {
        Candlestick<T> candle_prev = {data.high[idx-1], data.low[idx-1], 
            data.open[idx-1], data.close[idx-1], body_avg_ptr[idx-1], ma_ptr[idx-1]};

        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
    
        // Calculations for harami bull. Observe that both days body high 
        // and body low can't be the same, only one can be the same.
        if (harami_type == "bull" && candle_prev.has_long_body() &&
                candle_prev.is_red() && !candle.has_doji_body() && 
                candle.has_short_body() &&
                candle_prev.body_low <= candle.body_low && 
                candle_prev.body_high >= candle.body_high &&
                (candle_prev.body_low < candle.body_low || 
                 candle_prev.body_high > candle.body_high)) {

                    res_ptr[idx] = true;
        }

        // Calculations for harami bear. Observe that both days body high 
        // and body low can't be the same, only one can be the same.
        else if (harami_type == "bear" && candle_prev.has_long_body() &&
                candle_prev.is_green() && !candle.has_doji_body() &&
                candle.has_short_body() &&
                candle_prev.body_low <= candle.body_low && 
                candle_prev.body_high >= candle.body_high &&
                (candle_prev.body_low < candle.body_low || 
                 candle_prev.body_high > candle.body_high)) {

                    res_ptr[idx] = true;
        }

    }

    return res;
}

/*
 * Implementation of KICKING.
 *  
 * Params:
 *  high (py::array_t<T>) : Array with high prices.
 *  low (py::array_t<T>) : Array with low prices.
 *  open (py::array_t<T>) : Array with opening prices.
 *  close (py::array_t<T>) : Array with close prices.
 *  trend_period (int) : Period for moving average in order to identify trend.
 *  kicking_type (string) : Specify what kind of kicking type that should
 *      be calculated. Can choose from 'bull' or 'bear'.
 *  shadow_margin (float) : Float specifying what margin should be allowed
 *      for the shadows. For example, by using shadow_marign = 5, one allows
 *      the upper/lower shadows to be as long as 5% of the body size.
 *
 * Definition:
 *  Kicking bull.
 *      - Previous candle needs to be a long red marubozu candle. 
 *      - Current candle needs to gap up from the previous candle.
 *      - Current candle needs to be a long green marubozu candle.
 *
 *  Kicking bear.
 *      - Previous candle needs to be a long green marubozu candle. 
 *      - Current candle needs to gap down from the previous candle.
 *      - Current candle needs to be a long red marubozu candle.
 *
 */
template <typename T>
py::array_t<bool> kicking_calc(const py::array_t<T> high, 
        const py::array_t<T> low, const py::array_t<T> open, 
        py::array_t<T> close, const int trend_period, const std::string kicking_type,
        const float shadow_margin) {

    Container<T> data = {high, low, open, close};
    
    auto res = py::array_t<bool>(data.size);
    auto *res_ptr = (bool *) res.request().ptr;
    init_false(res_ptr, data.size);
    
    // Get average to use as a comparison for body sizes.
    const int body_avg_period = 14;
    py::array_t<T> body_avg = calc_body_avg(open, close, body_avg_period);
    auto *body_avg_ptr = (T *) body_avg.request().ptr;

    // Get moving average values for identifying trend.
    py::array_t<T> ma =  sma_calc(close, trend_period);
    auto *ma_ptr = (T *) ma.request().ptr;
    
    // Check start index based on the different periods. Observe we need +1 since
    // this pattern requires two candlesticks.
    int start_idx = body_avg_period + 1;

    for (int idx = start_idx; idx < data.size; ++idx) {
        Candlestick<T> candle_prev = {data.high[idx-1], data.low[idx-1], 
            data.open[idx-1], data.close[idx-1], body_avg_ptr[idx-1], ma_ptr[idx-1]};

        Candlestick<T> candle = {data.high[idx], data.low[idx], 
            data.open[idx], data.close[idx], body_avg_ptr[idx], ma_ptr[idx]};
    
        // Calculations for kicking bull. 
        if (kicking_type == "bull" && candle_prev.has_long_body() &&
                candle_prev.is_red() && candle_prev.is_marubozu(shadow_margin) && 
                candle.has_long_body() && candle.is_green() && 
                candle.is_marubozu(shadow_margin) && (candle.low > candle_prev.high)) {

                    res_ptr[idx] = true;
        }

        // Calculations for kicking bear. 
        else if (kicking_type == "bear" && candle_prev.has_long_body() &&
                candle_prev.is_green() && candle_prev.is_marubozu(shadow_margin) && 
                candle.has_long_body() && candle.is_red() && 
                candle.is_marubozu(shadow_margin) && (candle.high < candle_prev.low)) {

                    res_ptr[idx] = true;
        }

    }

    return res;
}

PYBIND11_MODULE(_bullish, m) {
    m.def("hammer_calc", &hammer_calc<double>, "Hammer pattern");
    m.def("hammer_calc", &hammer_calc<double>, "Hammer pattern");

    m.def("doji_calc", &doji_calc<double>, "Doji pattern");
    m.def("doji_calc", &doji_calc<float>, "Doji pattern");

    m.def("dragonfly_doji_calc", &dragonfly_doji_calc<double>, "Dragonfly doji pattern");
    m.def("dragonfly_doji_calc", &dragonfly_doji_calc<float>, "Dragonfly doji pattern");

    m.def("marubozu_white_calc", &marubozu_white_calc<double>, "Marubozu white pattern");
    m.def("marubozu_white_calc", &marubozu_white_calc<float>, "Marubozu white pattern");

    m.def("spinning_top_white_calc", &spinning_top_white_calc<double>, "Spinning top white pattern");
    m.def("spinning_top_white_calc", &spinning_top_white_calc<float>, "Spinning top white pattern");

    m.def("engulfing_calc", &engulfing_calc<double>, "Engulfing pattern");
    m.def("engulfing_calc", &engulfing_calc<float>, "Engulfing pattern");

    m.def("harami_calc", &harami_calc<double>, "Harami pattern");
    m.def("harami_calc", &harami_calc<float>, "Harami pattern");

    m.def("kicking_calc", &kicking_calc<double>, "Kicking pattern");
    m.def("kicking_calc", &kicking_calc<float>, "Kicking pattern");
}

