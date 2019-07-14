#include "stdafx.h"

#include <algorithm>
#include <cmath>
#include "WavReader.h"
#include <fstream>
#include "Note.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "WavSamples.h"

using namespace std;

WavReader::WavReader()
{
}

void WavReader::Test()
{
    bool a = false;
}

void WavReader::ReadFile(_In_ const std::wstring& path, _Out_ WavSamples& samples)
{
    vector<byte> rawBytes;

    BytesFromFile(path, rawBytes);

    int numChannels = 0;
    int sampleRate = 0;
    int bitsPerSample = 0;
    GetEncodingFormat(rawBytes, samples);

    GetSamples(rawBytes, samples);

    bool a = false;
}

void WavReader::BytesFromFile(_In_ const std::wstring& path, _Out_ std::vector<byte>& rawBytes)
{
    rawBytes.clear();
    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.c_str(), L"rb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            // Get the file size
            fseek(pFile, 0, SEEK_END);
            long sizeOfFile = ftell(pFile);
            rewind(pFile);

            // Resize the buffer
            rawBytes.resize(sizeOfFile);

            fread(rawBytes.data(), sizeof(byte), sizeOfFile, pFile);
        }

        fclose(pFile);
    }
}

void WavReader::GetEncodingFormat(
    _In_ std::vector<byte>& rawBytes,
    _Out_ WavSamples& samples)
{
    memcpy_s(&samples._numChannels, sizeof(samples._numChannels), &rawBytes[22], 2 * sizeof(byte));
    memcpy_s(&samples._sampleRate, sizeof(samples._sampleRate), &rawBytes[24], 4 * sizeof(byte));
    memcpy_s(&samples._bytesPerSample, sizeof(samples._bytesPerSample), &rawBytes[34], 2 * sizeof(byte));

    // Parsing is not supported for wav files that don't have a byte boundary number of bits in a sample
    _ASSERT(samples._bytesPerSample % 8 == 0);

    // Bits per sample is actually in the file, so divide by 8
    samples._bytesPerSample /= 8;
}

void WavReader::GetSamples(
    _In_ std::vector<std::byte>& rawBytes,
    _Inout_ WavSamples& samples)
{
    samples._samples.resize(samples._numChannels);

    _ASSERT(samples._bytesPerSample <= sizeof(samples._samples[0][0]));

    const int numberOfSamples = (rawBytes.size() - 44) / (samples._numChannels * samples._bytesPerSample);

    for (int i = 0; i < samples._numChannels; i++)
    {
        samples._samples[i].resize(numberOfSamples);
    }

    // The data block starts at byte 44
    for (uint32_t i = 44; i < rawBytes.size(); i += samples._bytesPerSample)
    {
        int channel = ((i - 44) / samples._bytesPerSample) % samples._numChannels;
        int sampleNum = (i - 44) / (samples._bytesPerSample * samples._numChannels);
        
        switch (samples._bytesPerSample)
        {
        case sizeof(byte):
        {
            byte data{ 0 };
            memcpy_s(&data, samples._bytesPerSample, &rawBytes[i], samples._bytesPerSample);
            samples._samples[channel][sampleNum] = static_cast<int>(data);
            break;
        }
        case sizeof(short):
        {
            short data = 0;
            memcpy_s(&data, samples._bytesPerSample, &rawBytes[i], samples._bytesPerSample);
            samples._samples[channel][sampleNum] = data;
            break;
        }
        case sizeof(int):
        {
            memcpy_s(&samples._samples[channel][sampleNum], samples._bytesPerSample, &rawBytes[i], samples._bytesPerSample);
            break;
        }
        defualt:
            _ASSERT(false);
            break;
        }
    }
}