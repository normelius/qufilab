"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for volatility indicators.

"""
import numpy as np 

from qufilab.indicators._volatility import *


def bbands(data, period, deviation = 2):
    """
    Bollinger bands
    """
    return bbands_calc(data, period, deviation)

def kc(close, high, low, period = 20, period_atr = 20, deviation = 2):
    """
    Keltner channels
    """
    return kc_calc(close, high, low, period, period_atr, deviation)

def atr(prices, highs, lows, periods):
    """
    Average true range
    """
    return atr_calc(prices, highs, lows, periods)

def cv(highs, lows, period = 10, smoothing_period = 10):
    """
    Chaikin volatility
    """
    return cv_calc(highs, lows, period, smoothing_period)
