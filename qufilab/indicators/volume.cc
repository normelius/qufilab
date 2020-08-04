

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <numeric>
#include <omp.h>
#include <x86intrin.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "volume.h"
#include "trend.h"
#include "util.h"

namespace py = pybind11;

/*
    Accumulation Distribution (ACDI)

 */
py::array_t<double> acdi_calc(const py::array_t<double> prices,
        const py::array_t<double> highs, const py::array_t<double> lows,
        const py::array_t<double> volumes) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];
    py::array_t<double> acdi = py::array_t<double>(prices_buf.size);

    double *prices_ptr = (double *) prices_buf.ptr;
    double *highs_ptr = (double *) highs.request().ptr;
    double *lows_ptr = (double *) lows.request().ptr;
    double *volumes_ptr = (double *) volumes.request().ptr;
    double *acdi_ptr = (double *) acdi.request().ptr;
    init_nan(acdi_ptr, size);
    
    double ad = 0.0;
    for (int idx = 0; idx < size; ++idx) {
        double nominator = highs_ptr[idx] - lows_ptr[idx];
        // Santiy check, highs should never be higher than low.
        if (nominator > 0.0) {
        ad += (((prices_ptr[idx] - lows_ptr[idx]) - (highs_ptr[idx] - prices_ptr[idx])) / nominator) * 
            (double)volumes_ptr[idx];
        }
        
        acdi_ptr[idx] = ad;
    }

    return acdi;
}


/*
    On Balance Volume
    @param (py::array_t<double>) prices: Vector with prices.
    @param (py::array_t<double>) volumes: Vector with volumes.
 */

py::array_t<double> obv_calc(const py::array_t<double> prices, 
        const py::array_t<double> volumes) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];
    py::array_t<double> obv = py::array_t<double>(prices_buf.size);

    double *prices_ptr = (double *) prices_buf.ptr;
    double *volumes_ptr = (double *) volumes.request().ptr;
    double *obv_ptr = (double *) obv.request().ptr;
    init_nan(obv_ptr, size);
    obv_ptr[0] = volumes_ptr[0];

    for (int idx = 1; idx < size; ++idx) {
        if (prices_ptr[idx] > prices_ptr[idx-1]) {
            obv_ptr[idx] = obv_ptr[idx-1] + volumes_ptr[idx];
        }

        else if (prices_ptr[idx] < prices_ptr[idx-1]) {
            obv_ptr[idx] = obv_ptr[idx-1] - volumes_ptr[idx];
        }
        
        else {
            obv_ptr[idx] = obv_ptr[idx-1];
        }
    }

    return obv;
}


/*
    Chaikin Money FLow
    @param (vector<float>) prices: Vector with closing prices.
    @param (vector<highs>) highs: Vector with high prices.
    @param (vector<lows>) lows: Vector with low prices.
    @param (vector<volumes>) volumes: Vector with volumes.
    @param (int) periods: Number of periods. Standard 21.
 */
py::array_t<double> cmf_calc(const py::array_t<double> prices,
        const py::array_t<double> highs, const py::array_t<double> lows,
        const py::array_t<double> volumes, const int periods) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];

    double *prices_ptr = (double *) prices_buf.ptr;
    double *highs_ptr = (double *) highs.request().ptr;
    double *lows_ptr = (double *) lows.request().ptr;
    double *volumes_ptr = (double *) volumes.request().ptr;

    py::array_t<double> cmf = py::array_t<double>(prices_buf.size);
    py::array_t<double> ac = py::array_t<double>(prices_buf.size);
    double *cmf_ptr = (double *) cmf.request().ptr;
    double *ac_ptr = (double *) ac.request().ptr;

    init_nan(cmf_ptr, size);
    init_nan(ac_ptr, size);
    
    // Money Flow Multiplier.
    for (size_t idx = 0; idx < size; ++idx) {
        ac_ptr[idx] = (((prices_ptr[idx] - lows_ptr[idx]) - (highs_ptr[idx] - prices_ptr[idx])) / 
            (highs_ptr[idx] - lows_ptr[idx])) * volumes_ptr[idx];
    }

    for (size_t idx = periods; idx < size + 1; ++idx) {
        double sum = std::accumulate(ac_ptr + idx - periods, ac_ptr + idx, 0.0);
        double vol = std::accumulate(volumes_ptr + idx - periods, volumes_ptr + idx, 0.0);
        cmf_ptr[idx-1] = sum / vol;
    }
   
    return cmf;
}


/*
    Chaikin Indicator
    @param (py::array_t<double>) prices: Vector with closing prices.
    @param (py::array_t<double>) highs: Vector with high prices.
    @param (py::array_t<double>) lows: Vector with low prices.
    @param (py::array_t<double>) volumes: Vector with volumes.
 */
py::array_t<double> ci_calc(const py::array_t<double> prices,
        const py::array_t<double> highs, const py::array_t<double> lows,
        const py::array_t<double> volumes) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];
    double *prices_ptr = (double *) prices_buf.ptr;

    py::array_t<double> ci = py::array_t<double>(prices_buf.size);
    double *ci_ptr = (double *) ci.request().ptr;
    init_nan(ci_ptr, size);

    py::array_t<double> acdi = acdi_calc(prices, highs, lows, volumes);
    double *acdi_ptr = (double*) acdi.request().ptr;

    py::array_t<double> ema10 = ema_calc(acdi, 10);
    double *ema10_ptr = (double *) ema10.request().ptr;

    py::array_t<double> ema3 = ema_calc(acdi, 3);
    double *ema3_ptr = (double *) ema3.request().ptr;

    for (int idx = 9; idx < size; idx++) {
        ci_ptr[idx] = ema3_ptr[idx] - ema10_ptr[idx];
    }
    
    return ci;
}

/*
 *  Positive Volume Index
 *  Math: If volume_today > volume_yesterday:
 *              PVI = PVI_yesterday + ((Close - Close_yesterday) / Close_yesterday) * PVI_yesterday
 *      
 *  @param (vector<float>) prices: Vector with closing prices.
 *  @param (vector<volumes>) volumes: Vector with volumes.
 *
 */
py::array_t<double> pvi_calc(const py::array_t<double> prices,
        const py::array_t<double> volumes) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];
    py::array_t<double> pvi = py::array_t<double>(prices_buf.size);

    double *prices_ptr = (double *) prices_buf.ptr;
    double *volumes_ptr = (double *) volumes.request().ptr;
    double *pvi_ptr = (double *) pvi.request().ptr;

    init_nan(pvi_ptr, size);
    pvi_ptr[0] = 100.0;

    for (size_t idx = 1; idx < size; ++idx) {
        if (volumes_ptr[idx] > volumes_ptr[idx-1]) {
            pvi_ptr[idx] = pvi_ptr[idx-1] + ((prices_ptr[idx] - prices_ptr[idx-1]) 
                    / prices_ptr[idx-1]) * pvi_ptr[idx-1];
        }

        else {
            pvi_ptr[idx] = pvi_ptr[idx-1];
        }
    }
    
    return pvi;
}

/*
 *  Negative Volume Index
 *  Math: If volume_today > volume_yesterday:
 *              NVI = NVI_yesterday + ((Close - Close_yesterday) / Close_yesterday) * NVI_yesterday
 *      
 *  @param (vector<float>) prices: Vector with closing prices.
 *  @param (vector<volumes>) volumes: Vector with volumes.
 *
 */
py::array_t<double> nvi_calc(const py::array_t<double> prices,
        const py::array_t<double> volumes) {

    py::buffer_info prices_buf = prices.request();
    const int size = prices_buf.shape[0];
    py::array_t<double> nvi = py::array_t<double>(prices_buf.size);

    double *prices_ptr = (double *) prices_buf.ptr;
    double *volumes_ptr = (double *) volumes.request().ptr;
    double *nvi_ptr = (double *) nvi.request().ptr;

    init_nan(nvi_ptr, size);
    nvi_ptr[0] = 100.0;

    for (size_t idx = 1; idx < size; ++idx) {
        if (volumes_ptr[idx] < volumes_ptr[idx-1]) {
            nvi_ptr[idx] = nvi_ptr[idx-1] + ((prices_ptr[idx] - prices_ptr[idx-1]) 
                    / prices_ptr[idx-1]) * nvi_ptr[idx-1];
        }

        else {
            nvi_ptr[idx] = nvi_ptr[idx-1];
        }
    }
    
    return nvi;
}


PYBIND11_MODULE(volume, m) {
    m.def("acdi_calc", &acdi_calc, "Accumulation Distribution");
    m.def("obv_calc", &obv_calc, "On Balance Volume");
    m.def("cmf_calc", &cmf_calc, "Chaikin Money Flow");
    m.def("ci_calc", &ci_calc, "Chaikin Indicator");
    m.def("pvi_calc", &pvi_calc, "Positive Volume Index");
    m.def("nvi_calc", &nvi_calc, "Negative Volume Index");
}


