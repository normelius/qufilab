"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for momentum indicators.

"""
import time
import numpy as np 

from qufilab.indicators._momentum import *




def rsi(data, periods, rsi_type = "smoothed"):
    """
    Relative strength index
    """
    return rsi_calc(data, periods, rsi_type.lower())

def macd(data):
    """
    Macd
    """
    return macd_calc(data)

def willr(data, highs, lows, periods):
    """
    William's R
    """
    return willr_calc(data, highs, lows, periods)

def roc(data, period):
    """
    Price rate of change
    """
    return roc_calc(data, period)

def vpt(data, volumes):
    """
    Volume price trend
    """
    return vpt_calc(data, volumes)

def mi(data, periods):
    """
    Momentum indicator
    """
    return mi_calc(data, periods)

def apo(data, period_slow = 26, period_fast = 12, ma = "sma"):
    """
    Absolute price oscillator
    """
    if ma.lower() not in ["sma", "ema"]:
        raise ValueError("param 'ma' needs to be 'ema' or 'sma'")

    return apo_calc(data, period_slow, period_fast, ma.lower())

def bop(high, low, open_, close):
    """
    Balance of power
    """
    return bop_calc(high, low, open_, close)

def cmo(close, period):
    """
    Chande momentum indicator
    """
    return cmo_calc(close, period)

def mfi(high, low, close, volume, period):
    """
    Money flow index
    """
    return mfi_calc(high, low, close, volume, period)

def ppo(prices, period_fast = 12, period_slow = 26, ma_type = "ema"):
    """
    Percentage price oscillator
    """
    if ma_type.lower() not in ["sma", "ema"]:
        raise ValueError("Param 'ma_type' needs to be 'sma' or 'ema'")

    return ppo_calc(prices, period_fast, period_slow, ma_type)

#def stochastic(close, high, low, mode = "fast", period_k = 10, method = "ema"):
#    """
#    Returns %K and %D stochastics.
#
#    :param close: Closing prices.
#    :type close: List.
#    :param high: High prices.
#    :type high: List.
#    :param low: Low prices.
#    :type low: List.
#    :param mode: Specify whether to calculate 'fast' or 'slow' stochastic.
#    :type mode: Str.
#    :param period_k: Specify the number of periods used in the Stochastic (%K)
#        calculation.
#    :type period_k: Int.
#    :param method: Specify the method that is used to calculate Stochastic (%D) 
#        calculation.
#    :type method: Str.
#    :return tuple: A tuple containing (%K, %D).
#    """
#    if method and method.lower() not in ['sma', 'ema']:
#        raise ValueError("Param 'method' needs to be 'sma' or 'ema'.")
#    
#    if mode and mode.lower() not in ['fast', 'slow']:
#        raise ValueError("Param 'mode' needs to be 'fast' or 'slow'.")
#    
#    if not isinstance(period_k, int):
#        raise ValueError("Param 'period_k' needs to be an int.")
#
#    return stochastic_calc(close, high, low, mode, period_k, method)


def cci(close, high, low, period = 20):
    return cci_calc(close, high, low, period)

def aroon(high, low, period = 20):
    return aroon_calc(high, low, period)

#def tsi(close, period = 25, period_double = 13):
#    return tsi_calc(close, period, period_double)
