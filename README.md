# QufiLab
Qufilab is a fast and modern technical analysis library.

## Features
* Wide array of technical indicators.
* Candlestick pattern recognition.

## Installation
```bash
pip3 install qufilab
```

Documentation for QufiLab can be found at: <https://qufilab.readthedocs.io>

## Usage
**WARNING**: All of qufilab's technical indicators are implemented in c++
and a big part of the speed performance comes from the fact that no 
type conversion exist between python and c++. In order for this to work, numpy arrays
of type **numpy.dtype.float64 (double) or numpy.dtype.float32 (float)** are preferably used. Observe that all other types of numpy arrays still are accepted, however the retured numpy array will be converted into the type **numpy.dtype.float64**.

#### Indicators
```python
import qufilab as ql

# Load sample data.
data = ql.load_sample('MSFT')

# Calculate simple moving average with a period of 200.
sma = ql.sma(data['close'], 200)

# Calculate exponential moving average with a period of 200.
ema = ql.ema(data['close'], 200)

# Calculate bollinger bands with a period of 20 and two standard deviations from the mean.
upper_band, middle_band, lower_band = ql.bbands(data['close'], period = 20, deviation = 2)
```

#### Patterns

```python
import qufilab as ql

# Load sample data.
data = ql.load_sample('MSFT')

hammer = ql.hammer(data['high'], data['low'], data['open'], data['close'])

dragonfly_doji = ql.dragonfly_doji(data['high'], data['low'], data['open'], data['close'])
```


