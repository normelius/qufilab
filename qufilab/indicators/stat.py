"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for statistics indicators.

"""
import numpy as np 

from qufilab.indicators._stat import *

def std(data, period, normalize = True):
    """
    Standard deviation

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 or float32 containing the data
        to calculate standard deviation from.
    period : `int`
        Number of periods to be used.
    normalize : `bool`, optional
        Specify whether to normalize the standard deviation with 
        n - 1 instead of n.
        Default to True.

    Returns
    -------
    `ndarray`
        Array of type float64 or float32 containing the calculated
        standard deviation.
    """
    return std_calc(data, period, normalize)

def var(data, period, normalize = True):
    """
    Variance

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 or float32 containing the data
        to calculate the variance from.
    period : `int`
        Number of periods to be used.
    normalize : `bool`, optional
        Specify whether to normalize the standard deviation with 
        n - 1 instead of n.
        Default to True.

    Returns
    -------
    `ndarray`
        Array of type float64 or float32 containing the calculated
        variance.
    """
    return var_calc(data, period, normalize)

def cov(data, market, period, normalize = True):
    """
    Covariance

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 or float32 containing the data
        to calculate covariance from.
    market : `ndarray`
        Array of type float64 or float32 containing the data
        that serves as the market comparison.
    period : `int`
        Number of periods to be used.
    normalize : `bool`, optional
        Specify whether to normalize the standard deviation with 
        n - 1 instead of n.
        Defaults to True.

    Returns
    -------
    `ndarray`
        Array of type float64 or float32 containing the calculated
        covariance values.
    """
    return cov_calc(data, market, period, normalize)

def beta(data, market, period, normalize = False):
    """
    Beta

    Parameters
    ----------
    data : `ndarray`
        Array of type float64 or float32 containing the data
        to calculate beta from.
    market : `ndarray`
        Array of type float64 or float32 containing the data
        that serves as the market comparison.
    period : `int`
        Number of periods to be used.
    normalize : `bool`, optional
        Specify whether to normalize the standard deviation with 
        n - 1 instead of n.
        Defaults to False.

    Returns
    -------
    `ndarray`
        Array of type float64 or float32 containing the calculated
        beta values.
    """
    return beta_calc(data, market, period, normalize)

def pct_change(price, period):
    """
    Percentage change

    Parameters
    ----------
    price : `ndarray`
        Array of type float64 or float32 containing the prices.
    period : `int`
        Number of periods to be used.

    Returns
    -------
    `ndarray`
        Array of type float64 or float32 containing the calculated
        percentage change values.
    """
    return pct_change_calc(price, period)
