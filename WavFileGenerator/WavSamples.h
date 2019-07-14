#pragma once

#include <vector>

struct WavSamples
{
public:
    WavSamples();

    int _numChannels;
    int _sampleRate;
    int _bytesPerSample;
    std::vector<std::vector<int>> _samples;
};
