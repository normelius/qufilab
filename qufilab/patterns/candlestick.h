#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


/*
 *  Datatype for a single candlestick.
 *  It handles all the necessary comparisons
 *  for a single candlestick.
 */
template <typename T>
struct Candlestick {
    // Explicit values on the candlestick.
    T high;
    T low;
    T open;
    T close;
    T body_high;
    T body_low;
    T ma;

    // Distances on the candlestick.
    T body;
    T body_avg;
    T upper_shadow;
    T lower_shadow;
    T range;

    // Construct a single candlestick.
    Candlestick(T high, T low, T open, T close, T body_avg, T ma);
    
    // Check whether candlestick has a upper shadow.
    bool has_upper_shadow(const float shadow_margin = 5.0);
    
    // Check whether candlestick has a lower shadow.
    bool has_lower_shadow(const float shadow_margin = 5.0);

    // Check whether candlestick has a short body.
    bool has_short_body();

    // Check whether candlestick has a long body.
    bool has_long_body();

    // Check whether candlestick is positive, i.e. green.
    bool is_green();

    // Check whether candlestick is negative, i.e. red.
    bool is_red();

    // Check whether candlestick body should be considered a doji.
    bool has_doji_body(const float doji_pct = 5.0);

    // Check whether upper- and lower shadow are considered equal
    // in size. Margins can be specified, which indicates what 
    // percentage difference is tolerable between the two sizes.
    // By defualt, 66.7% percentage difference is used, i.e. one shadow
    // can be twice as big as the other.
    bool has_equal_shadows(const float equal_shadow_pct = 2.0 / 3);
    
    // Check whether candlestick is a marubozu candle.
    bool is_marubozu(const float shadow_margin);

    // Identify if candlestick is in a uptrend.
    bool has_up_trend();

};




#endif
