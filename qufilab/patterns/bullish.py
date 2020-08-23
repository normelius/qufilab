"""
@ QufiLab, 2020.
@ Anton Normelius

Python interface for bullish patterns.

"""
import numpy as np

from qufilab.patterns._bullish import *

def hammer(high, low, open_, close, date = None, period = 10, shadow_margin = 5.0):
    """
    Parameters
    ----------
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a hammer pattern has been found.
    period : `int`, optional
        Specifying number of periods for trend identification.
    shadow_margin : `float`, optional
        Specify what margin should be allowed for the shadows. By using for 
        example 5%, both the lower and upper shadow can be as long as 5%
        of the candlestick body size. This exist to allow some margin and not
        exclude the shadows entirely.

    Returns
    -------
    hammer : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    hammer_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.

    Examples
    --------
    >>> import qufilab as ql
    >>> import numpy as np
    >>> open_ = np.array([192.3, 191.95, 190.9, 188.1, 186.9, 177.4, \
180.5, 179.7, 179.55, 182.75])
    >>> high = np.array([193.75, 192.65, 191.5, 188.1, 187.25, 182.95, \
180.5, 181.95, 180.4, 186.4]) 
    >>> low = np.array([190.75, 188.6, 188.45, 185.2, 183.35, 176.0, 174.45, \
177.85, 177.3, 182.75])
    >>> close = np.array([191.75, 189.8, 189.1, 186.5, 185.0, 182.6, 178.0, \
179.2, 180.35, 184.65]
    >>> date = np.array(['2018-10-18', '2018-10-19', '2018-10-22', '2018-10-23', \
'2018-10-24', '2018-10-25', '2018-10-26', '2018-10-29', \
'2018-10-30' '2018-10-31']
    >>> pattern = ql.hammer(open_, high, low, close)
    [False False False False False False False False  True False]
    
    When parameter `date` is specified, return value is an array containing
    the dates when a hammer pattern occured.

    >>> pattern_dates = ql.hammer(open_, high, low, close, date)
    ['2018-10-30']

    Notes
    -----
    Observe that the lower shadow shall be bigger than 2x the body, but lower
    than 3x the body.

    .. image:: images/hammer.png

    """
    hammer_type = "hammer"
    hammer = hammer_calc(high, low, open_, close, period, hammer_type, shadow_margin)
    
    if date is None:
        return hammer
    
    hammer_dates = date[hammer.astype(bool)]
    return hammer_dates
    
def inverted_hammer(open_, high, low, close, date = None, period = 10):
    """
    Parameters
    ----------
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a hammer pattern has been found.
    period : `int`, optional
        Specifying number of periods for trend identification.

    Returns
    -------
    inverted_hammer : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    inverted_hammer_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.

    Notes
    -----
    Observe that the lower shadow shall be bigger than 2x the body, but lower
    than 3x the body.

    """
    hammer_type = "inverted_hammer"
    hammer = hammer_calc(open_, high, low, close, period, hammer_type)
    
    if date is None:
        return hammer
    
    hammer_dates = date[hammer.astype(bool)]
    return hammer_dates

def doji(high, low, open_, close, date = None, period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    period : `int`, optional
        Specifying number of periods for trend identification.

    Returns
    -------
    doji : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    doji_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    doji = doji_calc(high, low, open_, close)
    
    if date is None:
        return doji
    
    doji_dates = date[doji.astype(bool)]
    return doji_dates

def dragonfly_doji(high, low, open_, close, date = None, period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    period : `int`, optional
        Specifying number of periods for trend identification.

    Returns
    -------
    dragonfly_doji : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    dragonfly_doji_dates: `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    dragonfly_doji = dragonfly_doji_calc(high, low, open_, close, period)
    
    if date is None:
        return dragonfly_doji
    
    dragonfly_doji_dates = date[dragonfly_doji.astype(bool)]
    return dragonfly_doji_dates

def marubozu_white(high, low, open_, close, date = None, shadow_margin = 5.0, period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    shadow_margin : `float`, optional
        Specify what margin should be allowed for the shadows. By using for 
        example 5%, both the lower and upper shadow can be as high as 5%
        of the candlestick body size. This exist to allow some margin (not
        restrict to no shadow).
    period : `int`, optional
        Specifying number of periods for trend identification.

    Returns
    -------
    marubozu_white : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    marubozu_white_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    marubozu_white = marubozu_white_calc(high, low, open_, close, shadow_margin, period)
    
    if date is None:
        return marubozu_white
    
    marubozu_white_dates = date[marubozu_white.astype(bool)]
    return marubozu_white_dates

def spinning_top_white(high, low, open_, close, date = None, period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    period : `int`, optional
        Specifying number of periods for trend identification.

    Returns
    -------
    spinning_top_white : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    spinning_top_white_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    spinning_top_white = spinning_top_white_calc(high, low, open_, close, period)
    
    if date is None:
        return spinning_top_white
    
    spinning_top_white_dates = date[spinning_top_white.astype(bool)]
    return spinning_top_white_dates


def engulfing_bull(high, low, open_, close, date = None, trend_period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    engulfing : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    engulfing_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    engulfing_type = "bull"
    engulfing = engulfing_calc(high, low, open_, close, trend_period, engulfing_type)
    
    if date is None:
        return engulfing
    
    engulfing_dates = date[engulfing.astype(bool)]
    return engulfing_dates

def engulfing_bear(high, low, open_, close, date = None, trend_period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    engulfing : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    engulfing_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    engulfing_type = "bear"
    engulfing = engulfing_calc(high, low, open_, close, trend_period, engulfing_type)
    
    if date is None:
        return engulfing
    
    engulfing_dates = date[engulfing.astype(bool)]
    return engulfing_dates


def harami_bull(high, low, open_, close, date = None, trend_period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    harami_bull : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    harami_bull_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    harami_type = "bull"
    harami = harami_calc(high, low, open_, close, trend_period, harami_type)
    
    if date is None:
        return harami
    
    harami_dates = date[harami.astype(bool)]
    return harami_dates

def harami_bear(high, low, open_, close, date = None, trend_period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    harami_bear : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    harami_bear_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    harami_type = "bear"
    harami = harami_calc(high, low, open_, close, trend_period, harami_type)
    
    if date is None:
        return harami
    
    harami_dates = date[harami.astype(bool)]
    return harami_dates


def kicking_bull(high, low, open_, close, date = None, trend_period = 10, shadow_margin = 5.0):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.
    shadow_margin : `float`, optional
        Specify what margin should be allowed for the shadows. By using for 
        example 5%, both the lower and upper shadow can be as high as 5%
        of the candlestick body size. This exist to allow some margin (not
        restrict to no shadow).

    Returns
    -------
    kicking_bull : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    kicking_bull_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    kicking_type = "bull"
    kicking = kicking_calc(high, low, open_, close, trend_period, kicking_type, shadow_margin)
    
    if date is None:
        return kicking
    
    kicking_dates = date[kicking.astype(bool)]
    return kicking_dates

def kicking_bear(high, low, open_, close, date = None, trend_period = 10, shadow_margin = 5.0):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.
    shadow_margin : `float`, optional
        Specify what margin should be allowed for the shadows. By using for 
        example 5%, both the lower and upper shadow can be as high as 5%
        of the candlestick body size. This exist to allow some margin (not
        restrict to no shadow).

    Returns
    -------
    kicking_bear : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    kicking_bear_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    kicking_type = "bear"
    kicking = kicking_calc(high, low, open_, close, trend_period, kicking_type, shadow_margin)
    
    if date is None:
        return kicking
    
    kicking_dates = date[kicking.astype(bool)]
    return kicking_dates

def piercing(high, low, open_, close, date = None, trend_period = 10):
    """
    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    piercing : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    piercing_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    piercing = piercing_calc(high, low, open_, close, trend_period)
    
    if date is None:
        return piercing
    
    piercing_dates = date[piercing.astype(bool)]
    return piercing_dates

def tws(high, low, open_, close, date = None, trend_period = 10):
    """
    Three White Soldiers

    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    tws : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    tws_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    tws = tws_calc(high, low, open_, close, trend_period)
    
    if date is None:
        return tws
    
    tws_dates = date[tws.astype(bool)]
    return tws_dates

def abandoned_baby_bull(high, low, open_, close, date = None, trend_period = 10):
    """
    Abandoned Baby Bull

    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    pattern : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    pattern_dates : `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    type_ = "bull"
    pattern = abandoned_baby_calc(high, low, open_, close, trend_period, type_)
    
    if date is None:
        return pattern
    
    pattern_dates = date[pattern.astype(bool)]
    return pattern_dates

def abandoned_baby_bear(high, low, open_, close, date = None, trend_period = 10):
    """
    Abandoned Baby Bear

    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    pattern : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    pattern_dates: `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    type_ = "bear"
    pattern = abandoned_baby_calc(high, low, open_, close, trend_period, type_)
    
    if date is None:
        return pattern
    
    pattern_dates = date[pattern.astype(bool)]
    return pattern_dates


def belt_hold_bull(high, low, open_, close, date = None, trend_period = 10,
        shadow_margin = 5.0):
    """
    Belt Hold Bull

    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    pattern : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    pattern_dates: `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    type_ = "bull"
    pattern = belt_hold_calc(high, low, open_, close, trend_period, type_,
            shadow_margin)
    
    if date is None:
        return pattern
    
    pattern_dates = date[pattern.astype(bool)]
    return pattern_dates


def belt_hold_bear(high, low, open_, close, date = None, trend_period = 10,
        shadow_margin = 5.0):
    """
    Belt Hold Bear

    Parameters
    ----------
    high : `ndarray`
        Array of type float64 or float32 containing high prices.
    low : `ndarray`
        Array of type float64 or float32 containing low prices.
    open_ : `ndarray`
        Array of type float64 or float32 containing opening prices.
    close : `ndarray`
        Array of type float64 or float32 containing close prices.
    date : `ndarray`, optional
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a pattern has been found.
    trend_period : `int`, optional
        Specify number of periods for trend identification.

    Returns
    -------
    pattern : `ndarray`
        A numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise. 
    pattern_dates: `ndarray`
        A numpy ndarray containing the dates when a pattern has
        bee found. This only returns in case `date` parameter
        has been specified.
    """
    type_ = "bear"
    pattern = belt_hold_calc(high, low, open_, close, trend_period, type_,
            shadow_margin)
    
    if date is None:
        return pattern
    
    pattern_dates = date[pattern.astype(bool)]
    return pattern_dates
