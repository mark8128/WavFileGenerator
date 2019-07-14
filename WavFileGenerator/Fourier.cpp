#include "stdafx.h"

#include <complex>
#include "Fourier.h"

#define _USE_MATH_DEFINES
#include <math.h>

Fourier::Fourier()
{
}

void Fourier::ComputeFFT(_In_ const std::vector<int>& samples, _Out_ std::vector<std::complex<double>>& transform)
{
    const int N = samples.size();
    
    transform.clear();
    transform.resize(N);

    // For each k in [0, N-1]
    for (int k = 0; k < N; k++)
    {
        // Sum from n = 0 to N-1
        for (int n = 0; n < N; n++)
        {
            // xn * e^(-i*2*pi*k*n/N)
            // By Euler's formula: e^(i*x) = cos(x) + i*sin(x), in this case x = -2.pi.k.n/N
            double x = ((-2.0*M_PI*k*n) / N);
            std::complex<double> c(cos(x), sin(x));

            // Input sample values are real so complex multiplication is not needed.
            // std::complex will reduce the computation to scalar multiplication
            transform[k] += c * static_cast<double>(samples[n]);
        }
    }
}
