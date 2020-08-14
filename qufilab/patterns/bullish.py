"""
@ QufiLab, 2020.
@ Anton Normelius

Python interface for bullish patterns.

"""
import numpy as np

from qufilab.patterns._bullish import *

def hammer(open_, high, low, close, date = None):
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
    date : `ndarray`
        Array containing corresponding dates which can be used in case
        one wants to only return the dates where a hammer pattern has been found.

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
    hammer = hammer_calc(open_, high, low, close)
    
    if date is None:
        return hammer
    
    # If dates, return dates where pattern is found.
    hammer_dates = date[hammer.astype(bool)]
    return hammer_dates
    


def dragonfly_doji(open_, high, low, close, date = None, bottom_shadow = 10.0):
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
    bottom_shadow : `float`, optional
        Specify how long the lower shadow should be in comparison to the body
        of the candlestick. 
        Defaults to 10.

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
    dragonfly_doji = dragonfly_doji_calc(open_, high, low, close, bottom_shadow)
    
    if date is None:
        return dragonfly_doji
    
    # If dates, return dates where pattern is found.
    dragonfly_doji_dates = date[dragonfly_doji.astype(bool)]
    return dragonfly_doji_dates




