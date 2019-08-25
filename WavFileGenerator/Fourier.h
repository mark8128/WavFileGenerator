#pragma once
#include <vector>

class Fourier
{
public:
    Fourier();
    ~Fourier() = default;

    void ComputeFFT(_In_ const std::vector<int>& samples, _Out_ std::vector<std::complex<double>>& transform);
    
    void ComputeMultiThreadedFFT(
        _In_ const std::vector<int>& samples,
        _In_ int inverseComputationFraction,
        _Out_ std::vector<std::complex<double>>& transform);

    void ComputeMultiThreadedFFTForRange(
        _In_ const std::vector<int>& samples,
        _In_ int inverseComputationFraction,
        _In_ int startSample,
        _In_ int endSample,
        _Out_ std::vector<std::complex<double>>& transform);
private:

    std::complex<double> ComputeFFTAtIndex(_In_ const std::vector<int>& samples, int index, int startIndex, int maxIndex);

    void ComputeFFTForRange(
        _In_ const std::vector<int>& samples,
        _Inout_ std::vector<std::complex<double>>* pTransform,
        int startInterval,
        int endInterval,
        int startSample,
        int endSample);
};