"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for trend indicators.

"""
import time
import numpy as np

from qufilab.indicators._trend import *


def _validate_input(data = None, period = None):
    """
    General function to validate specified user parameters.
    """
    if data is not None:
        if not isinstance(data, np.ndarray) or data.dtype != np.float64:
            raise ValueError("Param 'data' needs to be of type numpy.ndarray with " \
                "dtype numpy.float64, i.e. double")

    if period is not None:
        if not isinstance(period, int) or period <= 0:
            raise ValueError("Param 'period' needs to be an int greater than zero")


# Trend interface
# ---------------
def sma(data, period):
    """
    Simple moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a numpy ndarray of type float64 with simple moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> import numpy as np
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> sma = ql.sma(data, 2)
    [nan 1.5 2.5 3.5 4.5]

    Notes
    -----
    The calculation of sma is a equally weighted mean for the last *n* days.

    .. math::
        sma_K = \\frac{price_K + price_{K-1} + ... + price_{K-(n-1)}}{n} =
        \\frac{1}{n}\sum_{i=0}^{n-1} price_{K-i}

    """
    #_validate_input(data = data, period = period)
    return sma_calc(data, period)

def sma_test(data, period):
    return sma_calc_test(data, period)

def ema(data, period):
    """
    Exponential moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with exponential moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> ema = ql.ema(data, 2)
    [nan 1.5 2.5 3.5 4.5]

    Notes
    -----
    The calculation of ema can be written as a power series:

    .. math:: ema_K = \\alpha[price_K + (1-\\alpha)price_{K-1} + \\
        (1-\\alpha)^2price_{K-2}  + ... +  (1-\\alpha)^{K-(n-1)}price_{K-(n-1)},

    which also can be written as:

    .. math::
        ema_K = ema_{K-1} + \\alpha[price_K - ema_{K-1}],

    where :math:`\\alpha = \\frac{2}{n+1}` is the multiplier and depends on
    the period *n*. Observe that for the first ema value, a simple moving average
    is used.
    """
    _validate_input(data = data, period = period)
    return ema_calc(data, period)


def dema(data, period):
    """
    Double exponential moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with double exponential moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> dema = ql.dema(data, 2)
    [nan nan 3. 4. 5.]

    Notes
    -----
    The calculation of dema depends on taking an ema of an ema, hence the name.
    Since an ema is calculated from an ema, one more value is needed in the
    original series to start calculating values, which is shown in the example above.

    .. math::
        dema_K = 2 \cdot ema_K - ema(ema_K)

    """
    _validate_input(data = data, period = period)
    return dema_calc(data, period)

def tema(data, period):
    """
    Triple exponential moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with triple exponential moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> tema = ql.tema(data, 3)
    [nan nan nan 4. 5.]

    Notes
    -----
    The triple exponential moving average is similar to the other exponential
    moving averages, however this one is utilizing three exponential moving averages.

    .. math::
        tema_K = 3 \cdot ema_K - 3 \cdot ema(ema_K) + ema(ema(ema_K))
    """
    _validate_input(data = data, period = period)
    return tema_calc(data, period)

def t3(data, period, volume_factor = 0.7):
    """
    T3 moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.
    volume_factor : `float`, optional
        Volume factor to be used when calculating the constants.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with t3 moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0])
    >>> t3 = ql.t3(data, 2)
    [nan  nan  nan  nan  nan  nan  nan  nan  nan 9.55]

    Notes
    -----
    The t3 moving average indicator utilizes many different forms of the
    exponential moving average and is calculated with:

    .. math::
        t3 = c_1\cdot e_6 + c_2\cdot e_5 + c_3\cdot e_4 + c_4\cdot e_3

        ema_1 = ema(price)

        ema_2 = ema(ema_1)

        ema_3 = ema(ema_2)

        ema_4 = ema(ema_3)

        ema_5 = ema(ema_4)

        ema_6 = ema(ema_5)

        c_1 = -a^3

        c_2 = 3a^2 + 3a^3

        c_3 = –6a^2–3a–3a^3

        c_4 = 1 + 3a + a^3 + 3a^2

    where *a* is the volume factor and is typically set to :math:`0.7`.

    """
    _validate_input(data = data, period = period)

    if volume_factor < 0:
        raise ValueError("Param 'volume_factor' needs to be bigger than zero")

    return t3_calc(data, period, volume_factor)


def tma(data, period):
    """
    Triangular moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with triangular moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0])
    >>> tma = ql.tma(data, 2)
    [nan 1.5 2.5 3.5 4.5 5.5 6.5 7.5 8.5 9.5]

    Notes
    -----
    The triangular moving average is calculated by taking an sma of an sma.

    .. math::
        tma = sma(sma(price, n_1), n_2)

    As seen in the formula above, two different periods are used in this
    implementation. If the user specified period is even:

    .. math::
        n_1 = \\frac{period}{2}

        n_2 = \\frac{period}{2} + 1

    If the user specified period is uneven:

    .. math::
        n_1 = n_2 = \\frac{period + 1}{2} (\\textrm{rounded up})
    """
    _validate_input(data = data, period = period)

    return tma_calc(data, period)

def smma(data, period):
    """
    Smoothed moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with triangular moving averages.
    """
    _validate_input(data = data, period = period)
    return smma_calc(data, period)

def lwma(data, period):
    """
    Linear weighted moving average

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 containing the data to be used.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with linear weighted moving averages.

    Examples
    --------
    >>> import qufilab as ql
    >>> data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> lwma = ql.lwma(data, 2)
    [nan 1.66666667 2.66666667 3.66666667 4.66666667]

    Notes
    -----
    The linear weighted moving average is calculated similar to the simple
    moving average, except that the prices aren't equally weighted, but
    linearly weighted with the highest weight going first and then decreasing
    linearly. This implementation utilizes the number of periods as the highest
    weight, and thereafter decreasing down to one.

    .. math:: lwma_K = \\frac{price_K \cdot w_n + price_{K-1} \cdot w_{n-1} \\
            + ... + price_{K-(n-1)} \cdot w_1}{\sum_{i=1}^n w_i},

    where :math:`w_1 = 1, w_2 = 2,... w_n = n`
    """
    _validate_input(data = data, period = period)
    return lwma_calc(data, period)

def wc(close, high, low):
    """
    Weighted close

    Parameters
    ----------
    close : `ndarray`
        Array of type float64 containing the closing prices to be used.
    high : `ndarray`
        Array of type float64 containing the high prices to be used.
    low : `ndarray`
        Array of type float64 containing the low prices to be used.

    Returns
    -------
    `ndarray`
        Returns a ndarray of type float64 with weighted close values.

    Examples
    --------
    >>> import qufilab as ql
    >>> close  = np.array([5.0, 15.0, 25.0, 35.0, 45.0])
    >>> high = np.array([10.0, 20.0, 30.0, 40.0, 50.0])
    >>> low = np.array([1.0, 11.0, 21.0, 31.0, 41.0])
    >>> wc = ql.wc(close, high, low)
    [5.25 15.25 25.25 35.25 45.25]

    Notes
    -----
    The weighted close is defined as:

    .. math:: wc_K = \\frac{2 \cdot close_K + high_K + low_K}{4}

    """
    if not isinstance(close, np.ndarray) or close.dtype != np.float64:
        raise ValueError("Param 'close' needs to be of type numpy.ndarray with " \
            "dtype numpy.float64, i.e. double")

    if not isinstance(high, np.ndarray) or high.dtype != np.float64:
        raise ValueError("Param 'high' needs to be of type numpy.ndarray with " \
            "dtype numpy.float64, i.e. double")

    if not isinstance(low, np.ndarray) or low.dtype != np.float64:
        raise ValueError("Param 'low' needs to be of type numpy.ndarray with " \
            "dtype numpy.float64, i.e. double")

    return wc_calc(close, high, low)



