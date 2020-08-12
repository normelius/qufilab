# QufiLab
Qufilab is a fast and modern quantitative finance library 
implemented with the excellent pybind11 library and hence
utilizing the performance given by c++.

## Features
* Wide array of technical indicators.

## Installation
```bash
Not yet implemented
```
Documentation for QufiLab can be found at: <https://qufilab.readthedocs.io>

## Usage
> **WARNING**: All of qufilab's technical indicators are implemented in c++
and a big part of the speed performance comes from the fact that no 
type conversion exist between python and c++. In order for this to work, 
**numpy arraysof type float64 (double) and float32 (float)** are used in the implementation, 
and hence no other types of arrays/lists are currently supported.

#### Indicators
```python
import qufilab as ql
import numpy as np

# Creates an ndarray with element type float64.
data = np.random.rand(1000000)

# Calculate sma with a period of 200.
sma = ql.sma(data, period = 200)

# Calculate bollinger bands with a period of 20 and two standard deviations from the mean.
upper_band, middle_band, lower_band = ql.bbands(data, period = 20, deviation = 2)
```



