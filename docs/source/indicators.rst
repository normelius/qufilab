Indicators
==========

.. warning::
    All of qufilab's technical indicators are implemented in c++ and a 
    big part of the speed performance comes from the fact that no 
    type conversion exist between python and c++. In order for this to work, 
    numpy arrays of type **float64 (double) or float32 (float)** are used in the implementation, 
    and hence no other types are currently supported.

.. currentmodule:: qufilab

Trend
*****
.. autosummary::
    dema 
    ema
    lwma
    sma
    smma
    t3
    tma
    wc

.. autofunction:: dema
.. autofunction:: ema
.. autofunction:: lwma
.. autofunction:: sma
.. autofunction:: smma
.. autofunction:: t3
.. autofunction:: tma
.. autofunction:: wc

Momentum
********
.. autosummary::  
    apo
    aroon
    bop 
    cci
    cmo
    macd
    mfi
    mi
    ppo
    rsi
    roc
    vpt
    willr

.. autofunction:: apo
.. autofunction:: aroon
.. autofunction:: bop
.. autofunction:: cci
.. autofunction:: cmo
.. autofunction:: macd
.. autofunction:: mfi
.. autofunction:: mi
.. autofunction:: ppo
.. autofunction:: rsi
.. autofunction:: roc
.. autofunction:: vpt
.. autofunction:: willr

 

Volatility
**********
.. autosummary::
    atr
    bbands
    cv
    kc

.. autofunction:: atr
.. autofunction:: bbands
.. autofunction:: cv
.. autofunction:: kc

Volume
******
.. autosummary::
    acdi
    ci
    cmf
    nvi
    obv
    pvi

.. autofunction:: acdi
.. autofunction:: ci
.. autofunction:: cmf
.. autofunction:: nvi
.. autofunction:: obv
.. autofunction:: pvi

Statistics
**********
.. autosummary::
    beta
    cov
    pct_change
    std
    var

.. autofunction:: beta
.. autofunction:: cov
.. autofunction:: pct_change
.. autofunction:: std
.. autofunction:: var


