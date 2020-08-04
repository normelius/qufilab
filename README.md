# QufiLab

## Features
Wide array of technical indicators.
## Installation
```bash
$ pip3 install qufilab
```

Documentation can be found at: <https://qufilab.readthedocs.io>

## Usage
#### Indicators
```python
import qufilab as ql

# Creates an ndarray with element type float64.
data = np.random.rand(1000000)

# Calculate sma with a period of 200.
sma_calc = ql.sma(data, period = 200)

# Calculate bollinger bands with a period of 20 and two standard deviations from the mean.
upper_band, middle_band, lower_band = bbands(data, period = 20, deviation = 2)
```



