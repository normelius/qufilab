
"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for statistics indicators.

"""
import numpy as np 

from qufilab.indicators._stat import *

def std(data, period, normalize = True):
    return std_calc(data, period, normalize)

def var(data, period, normalize = True):
    return var_calc(data, period, normalize)

def cov(data, market, period, normalize = True):
    return cov_calc(data, market, period, normalize)

def beta(prices, market, period, normalize = False):
    return beta_calc(prices, market, period, normalize)

def pct_change(prices, period):
    return pct_change_calc(prices, period)
