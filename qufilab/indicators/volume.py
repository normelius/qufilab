"""
@ Qufilab, 2020.
@ Anton Normelius

Python interface for volume indicators.

"""
import time
import numpy as np 

from qufilab.indicators._volume import *



def acdi(prices, highs, lows, volumes):
    return acdi_calc(prices, highs, lows, volumes)

def obv(prices, volumes):
    return obv_calc(prices, volumes)

def cmf(prices, highs, lows, volumes, periods = 21):
    return cmf_calc(prices, highs, lows, volumes, periods)

def ci(prices, highs, lows, volumes):
    return ci_calc(prices, highs, lows, volumes)

def pvi(prices, volumes):
    return pvi_calc(prices, volumes)

def nvi(prices, volumes):
    return nvi_calc(prices, volumes)
