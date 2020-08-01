# Author: norme
import sys
import numpy as np

import unittest

import talib
import quant


class TestQuant(unittest.TestCase):

    @classmethod
    def setUpClass(self):
        self.close = np.random.rand(1000000)
        self.high = np.random.rand(1000000)
        self.low = np.random.rand(1000000)
        self.open = np.random.rand(1000000)
        self.volume = np.random.rand(1000000)
        self.tolerance = 1e-5

    def test_sma(self):
        """
        Test simple moving average.
        """
        periods = 200
        sma_quant = quant.sma(self.close, periods)
        sma_talib = talib.SMA(self.close, periods)
        np.testing.assert_allclose(sma_quant, sma_talib, rtol = self.tolerance)

    def test_ema(self):
        """
        Test exponential moving average.
        """
        periods = 200
        ema = quant.ema(self.close, periods)
        ema_talib = talib.EMA(self.close, periods)
        np.testing.assert_allclose(ema, ema_talib, rtol = self.tolerance)

    def test_dema(self):
        """
        Test Double Exponential Moving Average.
        """
        periods = 200
        dema = quant.dema(self.close, periods)
        dema_talib = talib.DEMA(self.close, periods)
        np.testing.assert_allclose(dema, dema_talib, rtol = self.tolerance)

    def test_tema(self):
        """
        Test Triple Exponential Moving Average.
        """
        periods = 200
        tema = quant.tema(self.close, periods)
        tema_talib = talib.TEMA(self.close, periods)
        np.testing.assert_allclose(tema, tema_talib, rtol = self.tolerance)

    def test_t3(self):
        """
        Test T3 Moving Average.
        """
        periods = 200
        t3 = quant.t3(self.close, periods)
        t3_talib = talib.T3(self.close, periods)
        np.testing.assert_allclose(t3, t3_talib, rtol = self.tolerance)

    def test_tma(self):
        """
        Test Triangular Moving Average.
        """
        periods = 200
        tma = quant.tma(self.close, periods)
        tma_talib = talib.TRIMA(self.close, periods)
        np.testing.assert_allclose(tma, tma_talib, rtol = self.tolerance)

    def test_lwma(self):
        """
        Test linear weighted moving average.
        """
        periods = 200
        lwma = quant.lwma(self.close, periods)
        lwma_talib = talib.WMA(self.close, periods)
        np.testing.assert_allclose(lwma, lwma_talib, rtol = self.tolerance)
    
    def test_wc(self):
        """ 
        Test weighted close.
        """
        wc_quant = quant.wc(self.close, self.high, self.low)
        wc_talib = talib.WCLPRICE(self.high, self.low, self.close)
        np.testing.assert_allclose(wc_quant, wc_talib, rtol = self.tolerance)
        
    def test_rsi(self):
        """
        Test relative strength index.
        """
        periods = 200
        rsi_quant = quant.rsi(self.close, periods)
        rsi_talib = talib.RSI(self.close, periods)
        np.testing.assert_allclose(rsi_quant, rsi_talib, rtol = self.tolerance)

    def test_william_r(self):
        """
        Test William's %R.
        """
        periods = 200
        william_quant = quant.willr(self.close, self.high, self.low, periods)
        william_talib = talib.WILLR(self.high, self.low, self.close, periods)
        np.testing.assert_allclose(william_quant, william_talib, rtol = self.tolerance)
    
    def test_momentum(self):
        """
        Test Momentum Indicator.
        """
        periods = 200
        mi_quant = quant.mi(self.close, periods)
        mi_talib = talib.MOM(self.close, periods)
        np.testing.assert_allclose(mi_quant, mi_talib, rtol = self.tolerance)

    def test_roc(self):
        """
        Test Rate-of-Change.
        """
        periods = 200
        q = quant.roc(self.close, periods)
        t = talib.ROC(self.close, periods)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_aroon(self):
        """
        Test Aroon oscillator.
        """
        periods = 200
        q = quant.aroon(self.high, self.low, periods)
        t = talib.AROONOSC(self.high, self.low, periods)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_obv(self):
        """
        Test On Balance Volume.
        """
        q = quant.obv(self.close, self.volume)
        t = talib.OBV(self.close, self.volume)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_cci(self):
        """
        Test Commodity Channel Index.
        """
        q = quant.cci(self.close, self.high, self.low, 200)
        t = talib.CCI(self.high, self.low, self.close, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_std(self):
        """
        Test Standard Deviation.
        """
        q = quant.std(self.close, 200, False)
        t = talib.STDDEV(self.close, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_var(self):
        """
        Test Variance.
        """
        q = quant.var(self.close, 200, False)
        t = talib.VAR(self.close, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_beta(self):
        """
        Test Beta.
        """
        q = quant.beta(self.close, self.high, 200)
        t = talib.BETA(self.high, self.close, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_acdi(self):
        """
        Test (Chaikin) Accumulation/Distribution line (ACDI).
        """
        q = quant.acdi(self.close, self.high, self.low, self.volume)
        t = talib.AD(self.high, self.low, self.close, self.volume)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_bbands(self):
        """
        Test Boolinger Bands.
        """
        q = quant.bbands(self.close, 200)
        t = talib.BBANDS(self.close, 200)
        np.testing.assert_allclose(q[0], t[0], rtol = self.tolerance)
        np.testing.assert_allclose(q[1], t[1], rtol = self.tolerance)
        np.testing.assert_allclose(q[2], t[2], rtol = self.tolerance)

    def test_atr(self):
        """
        Test Average True Range (ATR):
        """
        q = quant.atr(self.close, self.high, self.low, 200)
        t = talib.ATR(self.high, self.low, self.close, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_apo(self):
        """
        Test Absolute Price Oscillator (APO).
        """
        q = quant.apo(self.close, 200, 100, "sma")
        t = talib.APO(self.close, 200, 100, 0)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_bop(self):
        """
        Test Balance of Power (BOP).
        """
        q = quant.bop(self.high, self.low, self.open, self.close)
        t = talib.BOP(self.open, self.high, self.low, self.close)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_mfi(self):
        """
        Test Money Flow Index.
        """
        q = quant.mfi(self.high, self.low, self.close, self.volume, 200)
        t = talib.MFI(self.high, self.low, self.close, self.volume, 200)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

    def test_ppo(self):
        """
        Test Percentage Price Oscillator.
        """
        q = quant.ppo(self.close)
        t = talib.PPO(self.close, matype = 1)
        np.testing.assert_allclose(q, t, rtol = self.tolerance)

if __name__ == '__main__':
    unittest.main()

