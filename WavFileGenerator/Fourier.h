#pragma once
#include <vector>

class Fourier
{
public:
    Fourier();
    ~Fourier() = default;

    void ComputeFFT(_In_ const std::vector<int>& samples, _Out_ std::vector<std::complex<double>>& transform);
private:
};