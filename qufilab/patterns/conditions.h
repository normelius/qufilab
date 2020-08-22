/*
 *  This file handles all conditions for the different patterns.
 *  If one would like to see how a specific pattern is implemented,
 *  then this file is the right place to look at.
 */


#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "candlestick.h"

/*
 *  Conditions for HAMMER.
 *  
 *  Params: 
 *      candle (Candlestick<T>) : A struct candlestick containing the 
 *          data for a single candlestick.
 *      shadow_margin (T) : Specify shadow margin for the upper shadow.
 *      type (T) : Specify whether 'hammer' or 'inverted_hammer' should
 *          be calculated.
 *
 *  Definition for this implementation:
 *  Hammer definition
 *      - Small body, i.e. candlestick body size is smaller than
 *          the 14 period ema body size.
 *      - Observe that the body cannot be a doji body, since it then will be
 *          a dragonfly doji.
 *      - No/short upper shadow, i.e. upper shadow less than 5% of the body.
 *      - Lower shadow bigger than 2x the body size. Observe that some references
 *          specify that the lower shadow should be between 2-3 times the body.
 *
 *  Inverted Hammer definition
 *      - Small body, i.e. candlestick body size is smaller than
 *          the 14 period ema body size. 
 *      - Observe that the body cannot be a doji body, since it then will be
 *          a gravestone doji.
 *      - No/short lower shadow, i.e. lower shadow less than 5% of the body.
 *      - Upper shadow bigger than 2x the body size. Observe that some references
 *          specify that the upper shadow should be between 2-3 times the body.
 */
template <typename T>
bool hammer_conditions(Candlestick<T> candle, const T shadow_margin, 
        std::string type) {

    bool candle_conditions;

    if (type == "hammer") {
        candle_conditions = candle.has_short_body() && 
            !candle.has_doji_body() && 
            !candle.has_upper_shadow(shadow_margin) &&
            candle.lower_shadow >= candle.body * 2;
    }

    else if (type == "inverted_hammer") {
        candle_conditions = candle.has_short_body() &&
            !candle.has_doji_body() && 
            !candle.has_lower_shadow(shadow_margin) &&
            candle.upper_shadow >= candle.body * 2;
    }

    return candle_conditions ? true : false;
}


/*
 *  Conditions for DOJI.
 *
 *  Param:
 *      candle (Candlestick<T>) : Struct containing data for a single candlestick.
 *
 *  Definition:
 *      - Candlestick has a doji body, i.e. implemented here as a body
 *          smaller than 5% of the whole candlestick range. See implementation
 *      - Upper shadow and lower shadow are basically equal. This is handled
 *          by has_equal_shadows method. Some margins are
 *          required to catch more doji patterns.
 */
template <typename T>
bool doji_conditions(Candlestick<T> candle) {
    return candle.has_doji_body() && candle.has_equal_shadows() ? true : false;
}


/*
 *  Conditions for DRAGONFLY_DOJI
 *
 *  Param:
 *      candle (Candlestick<T>) : Struct containing data for a single candlestick.
 *
 *  Definition:
 *      - Candlestick has a doji body, i.e. implemented here as a body
 *          smaller than 5% of the whole candlestick range.
 *      - No/small upper shadow. Since body already is small in a doji  ,
 *          the specification here is that the shadow should be less than
 *          the body.
 */
template <typename T>
bool dragonfly_doji_conditions(Candlestick<T> candle) {
    bool candle_conditions = candle.has_doji_body() 
        && (candle.upper_shadow <= candle.body);

    return candle_conditions ? true : false;
}

/*
 *  Conditions for MARIBOZU_WHITE
 *
 *  Param:
 *      candle (Candlestick<T>) : Struct containing data for a single candlestick.
 *      shadow_margin (T) : Specify what shadow margin should be allowed
 *          for upper/lower shadow.
 *
 *  Definition:
 *      - Candlestick body should be long. This is implemented as being bigger
 *          than the 14 period ema of the body sizes.
 *      - Candlestick should have no (or very little) upper and lower shadow.
 *          This implementation doesn't strictly enforce zero shadows, but
 *          using a small margin which can be specified with the 'shadow_margin'
 *          parameter.
 */
template <typename T>
bool maribozu_white_conditions(Candlestick<T> candle, T shadow_margin) {
    bool candle_conditions = candle.has_long_body() && 
        !candle.has_upper_shadow(shadow_margin) && 
        !candle.has_lower_shadow(shadow_margin);

    return candle_conditions ? true : false;
}

/*
 *  Conditions for SPINNING_TOP_WHITE
 *
 *  Param:
 *      candle (Candlestick<T>) : Struct containing data for a single candlestick.
 *
 *  Definition:
 *      - Candlestick needs to be green, i.e. it is a positive candle.
 *      - Candlestick body can't be a doji.
 *      - Upper and lower shadows needs to be greater than the body. By calculating
 *          using the range of the candlestick, it is ensured that the shadows
 *          are quite symmetrical in size, which is what we want. Another approach
 *          to the implementation is to just check that the shadows are greater than
 *          the body size; however, this will have the effect that one shadow can be
 *          greatly bigger than another. Since this implementation assumes we want
 *          symmetry (at least to some extent), the range is used instead.
 */
template <typename T>
bool spinning_top_white_conditions(Candlestick<T> candle) {
    bool conditions = !candle.has_doji_body() && candle.is_green() &&
        candle.upper_shadow >= candle.range * 1.0/3 &&
        candle.lower_shadow >= candle.range * 1.0/3;

    return conditions ? true : false;
}

/*
 *  Conditions for ENGULFING.
 */
template <typename T>
bool engulfing_conditions(Candlestick<T> candle, Candlestick<T> candle_prev,
        const std::string type) {
    bool candle_cond, candle_prev_cond;

    if (type == "bull") {
        candle_prev_cond = candle_prev.is_red() && candle_prev.has_short_body();

        candle_cond = candle.is_green() && candle.has_long_body() &&
        (candle.open <= candle_prev.close && candle.close >= candle_prev.open) &&
        (candle.open < candle_prev.close || candle.close > candle_prev.open);
    }
            
    else if (type == "bear") {
        candle_prev_cond = candle_prev.is_green() && candle_prev.has_short_body();

        candle_cond = candle.is_red() && candle.has_long_body() &&
        (candle.open >= candle_prev.close && candle.close <= candle_prev.open) &&
        (candle.open > candle_prev.close || candle.close < candle_prev.open);
    }

    return candle_prev_cond && candle_cond ? true : false;
}


/*
 *  Conditions for HARAMI.
 *
 *  Definition:
 *  Observe that there exists numerous different interpretations of this pattern,
 *      read the definition below to see how this is implemented.
 *
 *  Harami bull.
 *      - Previous candle needs to be to be a long red candle.
 *      - Current candle needs to be within the body range of the previous candle.
 *          The current body high can be the same as previous body high, or current
 *          body low can be the same as previous body low, but now both at the
 *          same time.
 *      - In this implementation, current candle needs to be small, 
 *          however NOT a doji.
 *      - Current candle color does not matter.
 *
 *  Harami bear.
 *      - Previous candle needs to be to be a long green candle.
 *      - Current candle needs to be within the body range of the previous candle.
 *          The current body high can be the same as previous body high, or current
 *          body low can be the same as previous body low, but now both at the
 *          same time.
 *      - In this implementation, current candle needs to be small, 
 *          however NOT a doji.
 *      - Current candle color does not matter.
 */
template <typename T>
bool harami_conditions(Candlestick<T> candle, Candlestick<T> candle_prev,
        std::string type) {

    bool candle_cond, candle_prev_cond;

    if (type == "bull") {
        candle_prev_cond = candle_prev.has_long_body() &&
            candle_prev.is_red() &&
            (candle_prev.body_low <= candle.body_low &&
            candle_prev.body_high >= candle.body_high) &&
            (candle_prev.body_low < candle.body_low ||
            candle_prev.body_high > candle.body_high);

        candle_cond = candle.has_short_body() && !candle.has_doji_body();
    }

    else if (type == "bear") {
        candle_prev_cond = candle_prev.has_long_body() &&
            candle_prev.is_green() &&
            (candle_prev.body_low <= candle.body_low &&
            candle_prev.body_high >= candle.body_high) &&
            (candle_prev.body_low < candle.body_low ||
            candle_prev.body_high > candle.body_high);

        candle_cond = candle.has_short_body() && !candle.has_doji_body();
    }

    return candle_cond && candle_prev_cond ? true : false;
}

/*
 *  Conditions for KICKING.
 *
 *  Definition:
 *      Kicking bull.
 *          - Previous candle needs to be a long red marubozu candle.
 *          - Current candle needs to gap up from the previous candle.
 *          - Current candle needs to be a long green marubozu candle.
 *
 *      Kicking bear.
 *          - Previous candle needs to be a long green marubozu candle.
 *          - Current candle needs to gap down from the previous candle.
 *          - Current candle needs to be a long red marubozu candle.
 */

template <typename T>
bool kicking_conditions(Candlestick<T> candle, Candlestick<T> candle_prev,
        const float shadow_margin, const std::string type) {

    bool candle_cond, candle_prev_cond;

    if (type == "bull") {
        candle_cond = candle.has_long_body() && candle.is_green() &&
            candle.is_marubozu(shadow_margin) && 
            (candle.low > candle_prev.high);

        candle_prev_cond = candle_prev.has_long_body() &&
            candle_prev.is_red() && candle_prev.is_marubozu(shadow_margin);
    }

    else if (type == "bear") {
        candle_cond = candle.has_long_body() && candle.is_red() &&
            candle.is_marubozu(shadow_margin) && 
            (candle.high < candle_prev.low);

        candle_prev_cond = candle_prev.has_long_body() &&
            candle_prev.is_green() && candle_prev.is_marubozu(shadow_margin);

    }

    return candle_cond && candle_prev_cond ? true : false;
}

/*
 *  Conditions for PIERCING.
 *
 *  Definition:
 *      - Requires two candles.
 *      - First candle needs to be a red one.
 *      - Second candle needs to gap down, i.e. open lower than previous candle
 *          closing price, and close in the previous red body, between the
 *          midpoint and top of the body.
 */

template <typename T>
bool piercing_conditions(Candlestick<T> candle, Candlestick<T> candle_prev) {

    bool conditions = candle_prev.is_red() && candle.is_green() &&
        (candle_prev.low > candle.open) && 
        (candle.close > candle_prev.body_mid) & 
        (candle.close < candle_prev.body_high);

    return conditions ? true : false;
}

/*
 *  Conditions for Three White Soldiers.
 *
 *  Definition:
 *      - Requires three candles.
 *      - Each candle needs to be a long green candle.
 *      - Each candle needs to open higher then previous open,
 *          and close higher than previous close.
 *      - Since the price should close near the high, upper shadow
 *          shouldn't exist or be very small.
 */
template <typename T>
bool tws_conditions(Candlestick<T> c1, Candlestick<T> c2,
        Candlestick<T> c3) {
    
    bool green = c1.is_green() && c2.is_green() && c3.is_green();
    
    bool long_body = c1.has_long_body() && c2.has_long_body() &&
        c3.has_long_body();

    bool correct_span = (c1.open > c2.open) && (c1.open < c2.close) && 
        (c1.close > c2.close) && (c2.open > c3.open) && 
        (c2.open < c3.close) && (c2.close > c3.close);
    
    bool upper_shadow = !c1.has_upper_shadow(5.0) && 
        !c2.has_upper_shadow(5.0) && !c3.has_upper_shadow(5.0);

    return green && long_body && correct_span && upper_shadow;
}

/*
 *  Conditions for Abandoned Baby.
 *
 *  Definition:
 *      Abandoned Baby Bull
 *          - First candle red.
 *          - Second candle is a doji with a high value below low of both side
 *              candles.
 *          - Third candle is green with shadows that gaps above the middle doji.
 *
 *      Abandoned Baby Bear
 *          - First candle green.
 *          - Second candle is a doji with a low value higher than the high 
 *              of both side candles.
 *          - Third candle is red with shadows that gaps below the middle doji.
 */
template <typename T>
bool abandoned_baby_conditions(Candlestick<T> c1, Candlestick<T> c2,
        Candlestick<T> c3, std::string type) {
    
    bool c1_conditions, c2_conditions, c3_conditions;

    if (type == "bull") {
        c3_conditions = c3.is_red();
        c2_conditions = c2.has_doji_body() && (c2.high < c3.low);
        c1_conditions = c1.is_green() & (c1.low > c2.high);
    }

    else if (type == "bear") {
        c3_conditions = c3.is_green();
        c2_conditions = c2.has_doji_body() && (c2.low > c3.high);
        c1_conditions = c1.is_red() & (c1.high < c2.low);
    }

    return c3_conditions && c2_conditions && c1_conditions ? true : false;
}


#endif
