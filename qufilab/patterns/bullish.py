"""
@ QufiLab, 2020.
@ Anton Normelius

Python interface for bullish patterns.

"""
import numpy as np

from qufilab.patterns._bullish import *

def hammer(open_, high, low, close, date = None):
    """
    Hammer

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
    `ndarray`:
        Returns a numpy ndarray of type bool specifying true whether
        a pattern has been found or false otherwise.

    Examples
    --------
    >>> import qufilab as ql
    >>> import numpy as np
    """
    pattern = hammer_calc(open_, high, low, close)
    
    if date is None:
        return pattern
    
    # If dates, return dates where pattern is found.
    return date[pattern.astype(bool)]
    







