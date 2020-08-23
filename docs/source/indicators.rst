Indicators
==========

.. warning::
    All of qufilab's technical indicators are implemented in c++ and a 
    big part of the speed performance comes from the fact that no 
    type conversion exist between python and c++. In order for this to work, 
    numpy arrays of type ``numpy.dtype.float64`` or ``numpy.dtype.float32``    
    are preferably used. Observe that all other types of numpy arrays are accepted, 
    however the returned numpy array will be converted into the type 
    ``numpy.dtype.float64``.

.. currentmodule:: qufilab

Trend
*****
Double Exponential Moving Average
---------------------------------
.. autofunction:: dema

Exponential Moving Average
--------------------------
.. autofunction:: ema

Linear Weighted Moving Average
------------------------------
.. autofunction:: lwma

Simple Moving Average
---------------------
.. autofunction:: sma

Smoothed Moving Average
-----------------------
.. autofunction:: smma

T3 Moving Average
-----------------
.. autofunction:: t3

Triangular Moving Average
-------------------------
.. autofunction:: tma

Weighted Close
--------------
.. autofunction:: wc

Momentum
********
Absolute Price Oscillator
-------------------------
.. autofunction:: apo

Aroon Indicator
---------------
.. autofunction:: aroon

Balance of Power
----------------
.. autofunction:: bop

Commodity Channel Index
-----------------------
.. autofunction:: cci

Chande Momentum Indicator
-------------------------
.. autofunction:: cmo

MACD
----
.. autofunction:: macd

Money Flow Index
----------------
.. autofunction:: mfi

Momentum Indicator
------------------
.. autofunction:: mi

Percentage Price Oscillator
---------------------------
.. autofunction:: ppo

Relative Strength Index
-----------------------
.. autofunction:: rsi

Price Rate of Change
--------------------
.. autofunction:: roc

Volume Price Trend
------------------
.. autofunction:: vpt

William's R
-----------
.. autofunction:: willr

 

Volatility
**********
Average True Range
------------------
.. autofunction:: atr

Bollinger Bands
---------------
.. autofunction:: bbands

Chaikin Volatility
------------------
.. autofunction:: cv

Keltner Channels
----------------
.. autofunction:: kc

Volume
******
Accumulation Distribution
-------------------------
.. autofunction:: acdi

Chaikin Indicator
-----------------
.. autofunction:: ci

Chaikin Money Flow
------------------
.. autofunction:: cmf

Negative Volume Index
---------------------
.. autofunction:: nvi

On Balance Volume
-----------------
.. autofunction:: obv

Positive Volume Index
---------------------
.. autofunction:: pvi

Statistics
**********
Beta
----
.. autofunction:: beta

Covariance
----------
.. autofunction:: cov

Percentage Change
-----------------
.. autofunction:: pct_change

Standard Deviation
------------------
.. autofunction:: std

Variance
--------
.. autofunction:: var


