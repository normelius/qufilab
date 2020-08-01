
clear all; close all; clc;
%format longG
format longG

data_old = [22.273 22.194 22.085 22.174 22.184 22.134 22.234 22.432 ...
    22.244 22.293 22.154 22.393 22.382 22.611 23.356 24.052 ...
    23.753 23.832 23.952 23.634 23.823 23.872 23.654 23.187 23.098 ...
    23.326 22.681 23.098 22.403 22.173 23.326 22.681 23.098 22.403 22.173 ...
    23.753 23.832 23.952 23.634 23.823 23.872 23.654 23.187 23.098];


data = [190.75 187.5 177.15 179.05 175.1 176.45 178.65 179.9 179.1 ...
    176.45 169.85 173.2 183.3 182.55 182.2 181.7 185.8 188.95 188.3 ...
    190.6 185.8 187.9 190.65 199.35 197.45 200.4 202.1 200.0 201.7 ...
    193.9 194.25 195.95 200.7 201.6 201.7 205.1 206.5 204.8 205.5 ...
    206.0 208.0 209.0 214.7 220.1 219.8 224.3 224.2 225.2 223.7 224.2];

high = [191.25 192.0 182.0 181.35 178.55 177.0 179.4 180.15 181.3 ...
    178.65 175.4 174.7 183.5 183.85 183.8 181.95 186.0 189.25 190.4 ...
    190.7 188.6 190.2 193.0 200.4 200.8 201.3 202.1 203.5 204.4 ...
    199.95 198.15 196.05 202.2 203.6 202.7 205.6 208.3 206.7 206.0 ...
    209.5 208.2 210.1 217.1 220.6 221.0 224.3 225.1 226.2 227.0 225.1];

low = [184.5 187.5 177.15 178.35 175.0 173.95 176.7 178.05 178.75 ...
    175.85 166.1 171.65 175.6 180.55 179.65 177.9 182.0 186.35 ...
    186.75 188.25 185.05 187.15 187.5 191.55 197.45 197.2 197.5 ...
    197.55 198.3 193.9 191.05 189.7 196.85 199.5 198.8 199.95 205.4 ...
    203.6 202.5 205.3 203.3 206.5 209.3 215.9 219.3 220.4 222.2 221.1 ...
    223.3 222.1];

volume = data;


% Standard deviation
periods = 10;
stddev = zeros(1, length(data));
for ii = 1:length(data)-periods+1
    stddev(ii+periods-1) = std(data(ii:ii+periods-1));
end


% SMA
sma = tsmovavg(data, 's', 10);
ema = tsmovavg(data, 'e', 10);
sum(data(:, 1:10), 2)/10;

ema_i = indicators_compare(data', 'ema', 10);
weights = [1 2 3 4 5 6 7 8 9 10];
lwma = tsmovavg(data, 'w', weights);

% MACD
[macd, signal] = macd(data);


% Bollinger
bottom = sma - (2 * stddev);
upper = sma + (2 * stddev);

% ATR
ATR_comp = indicators_compare([high' low' data'], 'atr', 10);

%ACDI
acdi = cumsum(((data-low) - (high - data))./(high - low) .* volume);

% On Balance Volume.
% Observer we use low here as volume (testing).
obv = indicators_compare([data', low'], 'obv');

% Chaikin Money Flow (CMF)
c = (((data - low) - (high - data)) ./ (high - low)) .* volume;
periods = 10;
cmf = NaN(1, periods-1);
for ii = periods:length(data)
    cmf(ii) = sum(c(ii-periods+1:ii))/sum(volume(ii-periods+1:ii));
end

% Chaikin Indicator
chosc = chaikosc(high, low, data, volume);

% Chaikin Volatility Indicator
chvol = chaikvolat([high', low']);

% Williams %R
william = willpctr(high, low, data, 10);
will = indicators_compare([high' low' data'], 'william', 10);

% Weighted close
wc = wclose(high, low, data);

% Price rate-of-change (roc)
proc = prcroc(data, 24);

% Price and volume trend
pvt = pvtrend(data, volume);

% Momentum Indicator
mom = tsmom(data', 10);

% RSI;
rsi1 = indicators_compare(data', 'rsi', 14);

% Positive Volume Index
pvi = posvolidx(data, volume, 100);

% Negative Volume Index
nvi = negvolidx(data, volume, 100);

% Fast stochastic
[pctk, pctd] = fpctkd(high', low', data', 10, 3);
stoch = indicators_compare([high', low', data'], 'ssto', 10,  3);
asd = stochosc([high' low' data']);

% Slow stochastic
[pctk, pctd] = spctkd(pctk, pctd);

% Keltner Channels
keltner = indicators_compare([high' low' data'], 'keltner', 10, 2, 10);

% ADX, NOT YET DONE.
adx = indicators_compare([high' low' data'], 'adx', 10);

% Commodity Channel Index (CCI)
cci = indicators_compare([high' low' data'], 'cci');

% Aroon indicator (AR)
aroon = indicators_compare([high' low'], 'aroon', 10);

% True Strength Index
tsi = indicators_compare(data', 'tsi');


% Variance
var1 = movstd(data', 5)
