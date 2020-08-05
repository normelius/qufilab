# QufiLab
Qufilab is a fast and modern quantitative finance library 
implemented with the excellent pybind11 library and hence
utilizing the performance given by c++.

## Features
* Wide array of technical indicators.

## Installation
```bash
$ pip3 install qufilab
```

## Usage
> **WARNING**: All of qufilab's technical indicators are implemented in c++
and a big part of the speed performance comes from the fact that no 
type conversions exist between python and c++. In order for this to work, 
**numpy ndarrays of type float64** are used in the implementation, and hence no other 
types of arrays/list are currently supported.

Documentation for all indicators can be found at: <https://qufilab.readthedocs.io>

#### Indicators
```python
import qufilab as ql

# Creates an ndarray with element type float64.
data = np.random.rand(1000000)

# Calculate sma with a period of 200.
sma = ql.sma(data, period = 200)

# Calculate bollinger bands with a period of 20 and two standard deviations from the mean.
upper_band, middle_band, lower_band = ql.bbands(data, period = 20, deviation = 2)
```



