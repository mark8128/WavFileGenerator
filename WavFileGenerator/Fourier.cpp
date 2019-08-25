#include "stdafx.h"

#include <complex>
#include "Fourier.h"
#include <future>

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

void Fourier::ComputeMultiThreadedFFT(
    _In_ const std::vector<int>& samples,
    _In_ int inverseComputationFraction,
    _Out_ std::vector<std::complex<double>>& transform)
{
    const int maxFFTIndex = samples.size() / inverseComputationFraction;

    transform.clear();
    transform.resize(maxFFTIndex);

    const int numThreads = 8;
    static_assert(numThreads > 0);

    // Scope the futures
    {
        std::vector<std::future<void>> futures;

        for (int i = 0; i < numThreads; i++)
        {
            const int startIndex = (maxFFTIndex * i) / numThreads;
            const int endIndex = (maxFFTIndex * (i + 1)) / numThreads;

            if ((i == numThreads) && (endIndex != maxFFTIndex))
            {
                // Sanity check
                __debugbreak();
            }

            futures.push_back(std::async(
                &Fourier::ComputeFFTForRange,
                this,
                samples,
                &transform,
                startIndex,
                endIndex,
                0,
                samples.size()));
        }
    }
}

void Fourier::ComputeMultiThreadedFFTForRange(
    _In_ const std::vector<int>& samples,
    _In_ int inverseComputationFraction,
    _In_ int startSample,
    _In_ int endSample,
    _Out_ std::vector<std::complex<double>>& transform)
{
    // The entire FFT doesn't need to be computed since most is outside the audible range
    // Of the sample range provided, reduce the range of k by the fraction provided
    const int maxFFTIndex = (endSample - startSample) / inverseComputationFraction;

    transform.clear();
    transform.resize(maxFFTIndex);

    const int numThreads = 8;
    static_assert(numThreads > 0);

    // Scope the futures
    {
        std::vector<std::future<void>> futures;

        for (int i = 0; i < numThreads; i++)
        {
            // Compute the bounds on k for each thread
            const int startInterval = ((maxFFTIndex * i) / numThreads);
            const int endInterval = ((maxFFTIndex * (i + 1)) / numThreads);

            futures.push_back(std::async(
                &Fourier::ComputeFFTForRange,
                this,
                samples,
                &transform,
                startInterval,
                endInterval,
                startSample,
                endSample));
        }
    }
}

void Fourier::ComputeFFTForRange(
    _In_ const std::vector<int>& samples,
    _Inout_ std::vector<std::complex<double>>* pTransform,
    int startInterval,
    int endInterval,
    int startSample,
    int endSample)
{
    // Compute the values of the FFT for the given range [startIndex, endIndex)
    for (int k = startInterval; k < endInterval; k++)
    {
        (*pTransform)[k] = ComputeFFTAtIndex(samples, startSample, k, endSample);
    }
}

std::complex<double> Fourier::ComputeFFTAtIndex(
    _In_ const std::vector<int>& samples,
    int startIndex,
    int index,
    int maxIndex)
{
    std::complex<double> agg = 0;
    const int range = (maxIndex - startIndex);

    // Sum from n = 0 to N-1 - "0" being the start index of the range the FFT is being performed on.
    // The shift is accounted for by subtracting startIndex below
    for (int n = 0; n < range; n++)
    {
        // xn * e^(-i*2*pi*k*n/N)
        // By Euler's formula: e^(i*x) = cos(x) + i*sin(x), in this case x = -2.pi.k.n/N
        const double x = ((-2.0*M_PI*index*(n)) / range);
        const std::complex<double> c(cos(x), sin(x));

        // Input sample values are real so complex multiplication is not needed.
        // std::complex will reduce the computation to scalar multiplication
        agg += c * static_cast<double>(samples[n + startIndex]);
    }

    return agg;
}