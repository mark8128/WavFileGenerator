#include "stdafx.h"

#include <algorithm>
#include <cmath>
#include "FileBuilder.h"
#include <fstream>
#include "Note.h"
#include <numeric>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

const short FileBuilder::BytesPerSample = 2;

const std::unordered_map<
    FileBuilder::SoundStyle,
    const std::function<double(FileBuilder&, double, int)>>& FileBuilder::SampleFunctions =
{
    { FileBuilder::SoundStyle::SineWave, &FileBuilder::SineWaveSample },
    { FileBuilder::SoundStyle::HarmonicSine, &FileBuilder::HarmonicSample },
    { FileBuilder::SoundStyle::LowHarmonicSine, &FileBuilder::LowHarmonicSample },
    { FileBuilder::SoundStyle::BlurredWaves, &FileBuilder::ManyCloseWaves },
    { FileBuilder::SoundStyle::RoundedFrequencies, &FileBuilder::RoundedFreqMult },
    { FileBuilder::SoundStyle::MattBassEFirstSecondPureHarmonics, &FileBuilder::MattBassPureHarmonics },
    { FileBuilder::SoundStyle::MattBassEFirstSecondBlendedHarmonics, &FileBuilder::MattBassBlendedHarmonics },
    { FileBuilder::SoundStyle::MattBassE2s3sPure, &FileBuilder::MattBass2sTo3sPure },
};

FileBuilder::FileBuilder() :
    _sampleRate(44100),
    _style(SoundStyle::RoundedFrequencies)
{

}

FileBuilder::FileBuilder(std::vector<Note> notes) :
    _notes(notes),
    _sampleRate(44100),
    _style(SoundStyle::RoundedFrequencies)
{
}

FileBuilder::FileBuilder(std::vector<Note> notes, FileBuilder::SoundStyle style) :
    _notes(notes),
    _sampleRate(44100),
    _style(style)
{
}

void FileBuilder::Test()
{
    vector<int> samples;
    GetSamplesFromNotes(&FileBuilder::SineWaveSample, samples);

    wstring path = L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\Vector2.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            //fwrite(samples.data(), sizeof(int), samples.size(), pFile);

            for (const auto& sample : samples)
            {
                fprintf(pFile, "%d\n", sample);
            }
        }

        fclose(pFile);
    }

    bool a = false;
}

void FileBuilder::ToBuffer(_Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();

    vector<byte> dataChunk;
    BuildDataChunk(dataChunk);

    vector<byte> formatChunk;
    BuildFormatChunk(formatChunk);

    vector<byte> riffHeader;
    BuildRIFFChunk(dataChunk.size() + formatChunk.size(), riffHeader);

    buffer.insert(buffer.begin(), riffHeader.begin(), riffHeader.end());
    buffer.insert(buffer.end(), formatChunk.begin(), formatChunk.end());
    buffer.insert(buffer.end(), dataChunk.begin(), dataChunk.end());
}

void FileBuilder::ToFile(std::wstring path)
{
    vector<byte> buffer;
    ToBuffer(buffer);
    
    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            fwrite(buffer.data(), sizeof(byte), buffer.size(), pFile);
        }

        fclose(pFile);
    }
}

void FileBuilder::BuildRIFFChunk(uint32_t totalChunkSize, _Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();
    buffer.resize(12);

    // The string values are encode in ACSII. To avoid ACSII conversion the hex is hard coded
    // However strings are encoded in big endian while integers in little endian. To ensure
    // the value is encoded in big endian the hex representation of the strings are written
    // in reverse.
    static const uint32_t riff = 0x46464952;
    static const uint32_t wave = 0x45564157;

    uint32_t sizeAfterThisPoint = totalChunkSize + 4;

    memcpy_s(buffer.data(), sizeof(uint32_t), &riff, sizeof(uint32_t));
    memcpy_s(&buffer[4], sizeof(uint32_t), &sizeAfterThisPoint, sizeof(uint32_t));
    memcpy_s(&buffer[8], sizeof(uint32_t), &wave, sizeof(uint32_t));
}

void FileBuilder::BuildFormatChunk(_Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();
    buffer.resize(24);

    // Once again the ASCII "fmt " string is encoded in hex backward so the integer being
    // encoded as little endian results in the string in big endian.
    static const uint32_t fmt = 0x20746d66;
    static const uint32_t remainingChunkSize = 16;
    static const short audioFormat = 1;
    static const short numChannels = 1;

    short blockAlign = numChannels * BytesPerSample;
    uint32_t byteRate = _sampleRate * blockAlign;
    short bitsPerSample = BytesPerSample * 8;

    memcpy_s(buffer.data(), sizeof(uint32_t), &fmt, sizeof(uint32_t));
    memcpy_s(&buffer[4], sizeof(uint32_t), &remainingChunkSize, sizeof(uint32_t));
    memcpy_s(&buffer[8], sizeof(short), &audioFormat, sizeof(short));
    memcpy_s(&buffer[10], sizeof(short), &numChannels, sizeof(short));
    memcpy_s(&buffer[12], sizeof(uint32_t), &_sampleRate, sizeof(uint32_t));
    memcpy_s(&buffer[16], sizeof(uint32_t), &byteRate, sizeof(uint32_t));
    memcpy_s(&buffer[20], sizeof(short), &blockAlign, sizeof(short));
    memcpy_s(&buffer[22], sizeof(short), &bitsPerSample, sizeof(short));
}

void FileBuilder::BuildDataChunk(_Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();

    // First get the data buffer so the size can be determined
    GetDataBuffer(buffer);

    // Reverse to correct for endian-ness
    static const uint32_t data = 0x61746164;
    uint32_t dataSize = buffer.size();

    vector<byte> headerValues;
    headerValues.resize(8);
    memcpy_s(headerValues.data(), sizeof(uint32_t), &data, sizeof(uint32_t));
    memcpy_s(&headerValues[4], sizeof(uint32_t), &dataSize, sizeof(uint32_t));

    buffer.insert(buffer.begin(), headerValues.begin(), headerValues.end());
}

void FileBuilder::GetDataBuffer(_Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();

    GenerateNotes(buffer);
}

void FileBuilder::GenerateNotes(_Out_ std::vector<std::byte>& buffer)
{
    buffer.clear();
    static_assert(sizeof(short) == BytesPerSample);

    // Since notes can overlap the maximum sample value is not known. It must be that |value| < 32768
    // so the values are first calculated in an int vector, normalized, then converted to the buffer
    vector<int> samples;
    GetSamplesFromNotes(SampleFunctions.at(_style), samples);

    // Normalize the values
    NormalizeValues(samples, buffer);
}

void FileBuilder::GetSamplesFromNotes(_In_ const std::function<double(FileBuilder&, double, int)>& sampler, _Out_ std::vector<int>& samples)
{
    samples.clear();

    const uint32_t numberOfSamples = GetTotalNumberOfSamples();
    samples.resize(numberOfSamples);

    // Iterate through each note and modify the assocaited sample accordingly
    for (const auto& note : _notes)
    {
        // Looking for the value closest to but not over _duration where _freq * v is an integer
        double nonClippedDuration = floor(2.0 * note._freq * note._duration) / (2.0 * note._freq);

        uint32_t startingSample = static_cast<uint32_t>(note._startTime * _sampleRate);
        uint32_t samplesInNote = static_cast<uint32_t>(/*nonClippedDuration*/note._duration * _sampleRate);

        // The duration of the amplitude modifier
        double ampModDuration = _sampleRate / 100.0;

        for (uint32_t i = 0; i < samplesInNote; i++)
        {
            // The amp mod factor allows a note to fade in and out so it rounds the tranistion curves between notes
            // The first 441 and last 441 samples will have reduced amplitude, linearly increase/decreasing to the correct value
            double ampModFactor = min(min((i / ampModDuration), ((samplesInNote - i) / ampModDuration)), 1.0);

            samples[startingSample + i] += static_cast<int>(ampModFactor * note._amp * sampler(*this, note._freq, i));
        }
    }
}

void FileBuilder::NormalizeValues(_In_ std::vector<int>& samples, _Out_ std::vector<std::byte>& buffer)
{
    static_assert(sizeof(short) == BytesPerSample);

    // Resize the output buffer based on the samples provided
    buffer.clear();
    buffer.resize(samples.size() * BytesPerSample);

    auto result = std::minmax_element(samples.begin(), samples.end());
    const int maxMagnitude = max(abs(*result.first), abs(*result.second));

    // 32760 is the maximum amplitude for a 16 bit sample.
    double normalizer = 32760.0 / maxMagnitude;

    // Normalize the values by max
    for (size_t i = 0; i < samples.size(); i++)
    {
        int normalized = static_cast<int>(samples[i] * normalizer);

        // Integer values (short) are little endian. Put the high order bit in the higher range in the buffer
        buffer[i*BytesPerSample] = static_cast<byte>(normalized);
        buffer[i*BytesPerSample + 1] = static_cast<byte>(normalized >> 8);
    }
}

uint32_t FileBuilder::GetTotalNumberOfSamples()
{
    static const auto pred = [](Note left, Note right) -> bool
    {
        return ((left._startTime + left._duration) < (right._startTime + right._duration));
    };

    Note last = *std::max_element(_notes.begin(), _notes.end(), pred);

    const double songDuration = last._startTime + last._duration;
    return static_cast<uint32_t>(songDuration * _sampleRate + 0.5 /* rounding */);
}

double FileBuilder::SineWaveSample(double freq, int sampleIndex)
{
    return sin(2.0 * M_PI * freq * sampleIndex / _sampleRate);
}

double FileBuilder::HarmonicSample(double freq, int sampleIndex)
{
    return sin(2.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.1 * sin(4.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.34 * sin(6.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.06 * sin(8.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.05 * sin(10.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.05 * sin(12.0 * M_PI * freq * sampleIndex / _sampleRate);
}
double FileBuilder::LowHarmonicSample(double freq, int sampleIndex)
{
    return sin(2.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(M_PI * freq * sampleIndex / _sampleRate) +
        0.25 * sin(4.0 * M_PI * freq * sampleIndex / _sampleRate);
}

double FileBuilder::ManyCloseWaves(double freq, int sampleIndex)
{
    // First harmonic
    double first = sin(2.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(2.0 * M_PI * (freq + 1) * sampleIndex / _sampleRate) +
        0.5 * sin(2.0 * M_PI * (freq - 1) * sampleIndex / _sampleRate) +
        0.25 * sin(2.0 * M_PI * (freq + 2) * sampleIndex / _sampleRate) +
        0.25 * sin(2.0 * M_PI * (freq - 2) * sampleIndex / _sampleRate);

    // Second harmonic
    double second = sin(4.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(4.0 * M_PI * (freq + 1) * sampleIndex / _sampleRate) +
        0.5 * sin(4.0 * M_PI * (freq - 1) * sampleIndex / _sampleRate) +
        0.25 * sin(4.0 * M_PI * (freq + 2) * sampleIndex / _sampleRate) +
        0.25 * sin(4.0 * M_PI * (freq - 2) * sampleIndex / _sampleRate);

    // Half harmonic
    double half = sin(M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(M_PI * (freq + 1) * sampleIndex / _sampleRate) +
        0.5 * sin(M_PI * (freq - 1) * sampleIndex / _sampleRate) +
        0.25 * sin(M_PI * (freq + 2) * sampleIndex / _sampleRate) +
        0.25 * sin(M_PI * (freq - 2) * sampleIndex / _sampleRate);

    return (first + (second / 4.0) + (half / 2.0));
}

double FileBuilder::RoundedFreqMult(double freq, int sampleIndex)
{
    // First harmonic
    double first = sin(2.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(2.0 * M_PI * (freq * 1.005) * sampleIndex / _sampleRate) +
        0.5 * sin(2.0 * M_PI * (freq * 0.995) * sampleIndex / _sampleRate) +
        0.25 * sin(2.0 * M_PI * (freq * 1.01) * sampleIndex / _sampleRate) +
        0.25 * sin(2.0 * M_PI * (freq * 0.99) * sampleIndex / _sampleRate);

    // Second harmonic
    double second = sin(4.0 * M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(4.0 * M_PI * (freq * 1.005) * sampleIndex / _sampleRate) +
        0.5 * sin(4.0 * M_PI * (freq * 0.995) * sampleIndex / _sampleRate) +
        0.25 * sin(4.0 * M_PI * (freq * 1.01) * sampleIndex / _sampleRate) +
        0.25 * sin(4.0 * M_PI * (freq * 0.99) * sampleIndex / _sampleRate);

    // Half harmonic
    double half = sin(M_PI * freq * sampleIndex / _sampleRate) +
        0.5 * sin(M_PI * (freq * 1.005) * sampleIndex / _sampleRate) +
        0.5 * sin(M_PI * (freq * 0.995) * sampleIndex / _sampleRate) +
        0.25 * sin(M_PI * (freq * 1.01) * sampleIndex / _sampleRate) +
        0.25 * sin(M_PI * (freq * 0.99) * sampleIndex / _sampleRate);

    return (first + (second / 4.0) /*+ (half / 2.0)*/);
}

double FileBuilder::MattBassPureHarmonics(double freq, int sampleIndex)
{
    static std::vector<double> ampModifiers = { 
        4.737,  // 1
        16.434, // 2
        37.369, // 3
        15.565, // 4
        0.0,    // 5
        3.58,   // 6
        6.580,  // 7
        12.190, // 8
        3.546,  // 9
    };
    
    double agg = 0.0;
    std::vector<double> harmonics;

    // Compute the harmonics
    for (size_t i = 0; i < ampModifiers.size(); i++)
    {
        harmonics.push_back(sin(i * 2.0 * M_PI * freq * sampleIndex / _sampleRate));
    }

    // Compute the dot product to generate the weighted result
    return std::inner_product(ampModifiers.begin(), ampModifiers.end(), harmonics.begin(), 0.0);
}

double FileBuilder::MattBassBlendedHarmonics(double freq, int sampleIndex)
{
    static std::vector<double> ampModifiers = {
    4.737,  // 1
    16.434, // 2
    37.369, // 3
    15.565, // 4
    0.0,    // 5
    3.58,   // 6
    6.580,  // 7
    12.190, // 8
    3.546,  // 9
    };

    double agg = 0.0;
    std::vector<double> harmonics;

    // Compute the harmonics
    for (size_t i = 0; i < ampModifiers.size(); i++)
    {
        const double data =
            0.0902 * sin(i * 2.0 * M_PI * freq * 0.984 * sampleIndex / _sampleRate) +
            0.1784 * sin(i * 2.0 * M_PI * freq * 0.992 * sampleIndex / _sampleRate) +
            0.4374 * sin(i * 2.0 * M_PI * freq * 1.000 * sampleIndex / _sampleRate) +
            0.2060 * sin(i * 2.0 * M_PI * freq * 1.008 * sampleIndex / _sampleRate) +
            0.0880 * sin(i * 2.0 * M_PI * freq * 1.016 * sampleIndex / _sampleRate);

        harmonics.push_back(data);
    }

    // Compute the dot product to generate the weighted result
    return std::inner_product(ampModifiers.begin(), ampModifiers.end(), harmonics.begin(), 0.0);
}

double FileBuilder::MattBass2sTo3sPure(double freq, int sampleIndex)
{
    static std::vector<double> ampModifiers = {
        5.8233,     // 1
        49.1718,    // 2
        28.5168,    // 3
        10.9535,    // 4
        0,          // 5
        0,          // 6
        3.5697,     // 7
        1.9650,     // 8
    };

    double agg = 0.0;
    std::vector<double> harmonics;

    // Compute the harmonics
    for (size_t i = 0; i < ampModifiers.size(); i++)
    {
        harmonics.push_back(sin(i * 2.0 * M_PI * freq * sampleIndex / _sampleRate));
    }

    // Compute the dot product to generate the weighted result
    return std::inner_product(ampModifiers.begin(), ampModifiers.end(), harmonics.begin(), 0.0);
}