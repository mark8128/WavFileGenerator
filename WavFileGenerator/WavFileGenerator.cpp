#include "stdafx.h"

#include <algorithm>
#include <complex>
#include "FileBuilder.h"
#include "Fourier.h"
#include "FurElise.h"
#include "Note.h"
#include "WavReader.h"
#include "WavSamples.h"

void ToCSV(const std::vector<int>&, const std::wstring&);
void ToCSV(const std::vector<std::complex<double>>&, const std::wstring&);
void ToCSV(const std::vector<std::pair<int, int>>& data, const std::wstring& fileName);
void ToMagnitudeCSV(const std::vector<std::complex<double>>&, const std::wstring&);

void GenerateWav()
{
    std::vector<Note> notes;

    Note concertA(440, 0, 2);
    Note middleC(261.6, 1, 2);

    notes.push_back(concertA);
    notes.push_back(middleC);

    notes.push_back(Note(Tone::C, 4, 4, 0.5));
    notes.push_back(Note(Tone::D, 4, 4.5, 0.5));
    notes.push_back(Note(Tone::E, 4, 5, 0.5));
    notes.push_back(Note(Tone::F, 4, 5.5, 0.5));
    notes.push_back(Note(Tone::G, 4, 6, 0.5));
    notes.push_back(Note(Tone::A, 4, 6.5, 0.5));
    notes.push_back(Note(Tone::B, 4, 7, 0.5));
    notes.push_back(Note(Tone::C, 5, 7.5, 0.5));

    FurElise::Generate(notes);

    FileBuilder builder(notes);
    builder.ToFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\FurEliseMultRounding.wav");
}

void GenerateFFTTestWav()
{
    std::vector<Note> notes;

    // Frequency granularity of the FFT is inversly proportional to the duration.
    Note concertA(440, 0, 1);
    Note middleC(261.6, 1, 0.5);

    notes.push_back(concertA);
    notes.push_back(middleC);
    /*
    notes.push_back(Note(Tone::C, 4, 0.25, 0.05));
    notes.push_back(Note(Tone::D, 4, 0.3, 0.05));
    notes.push_back(Note(Tone::E, 4, 0.35, 0.05));
    notes.push_back(Note(Tone::F, 4, 0.4, 0.05));
    notes.push_back(Note(Tone::G, 4, 0.45, 0.05));
    notes.push_back(Note(Tone::A, 4, 0.5, 0.05));
    notes.push_back(Note(Tone::B, 4, 0.55, 0.05));
    notes.push_back(Note(Tone::C, 5, 0.6, 0.05));*/

    FileBuilder builder(notes);
    builder.ToFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\BassRecording\\RangeFFTTest.wav");
}

void GenerateMattBass()
{
    std::vector<Note> notes;

    // Frequency granularity of the FFT is inversly proportional to the duration.
    Note E1(41, 0, 3);

    notes.push_back(E1);

    FileBuilder builder(notes, FileBuilder::SoundStyle::MattBassE2s3sPure);
    builder.ToFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\BassEmulation\\E1.2sTo3s.Pure.wav");
}

void ProcessRecording(const std::wstring& fileName, const std::wstring& outputUniquifier, int startSample, int endSample)
{
    WavReader reader;
    WavSamples samples;

    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\BassRecording\\" << fileName << ".wav";

    reader.ReadFile(path.str().c_str(), samples);

    ToCSV(samples._samples[0], fileName);

    Fourier fourier;
    std::vector<std::complex<double>> fft;

    // Fourier granularity is sample rate / size. Sample rate is 44100 (defined in wav files used in this application)
    // The time of the note is inversely proportional to the granularity, so the axis should be plotted as 1/N.
    //fourier.ComputeFFT(samples._samples[0], fft);
    fourier.ComputeMultiThreadedFFTForRange(
        samples._samples[0],
        8,              // Process on 8 threads
        startSample,    // Start the FFT on this sample
        endSample,      // Only proccess to this sample
        fft);

    std::wostringstream outputFileName;
    outputFileName << fileName << outputUniquifier;
    ToCSV(fft, outputFileName.str());
    ToMagnitudeCSV(fft, outputFileName.str());
}

void ToCSV(const std::vector<int>& data, const std::wstring& fileName)
{
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\" << fileName << "Vector.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.str().c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            //fwrite(samples.data(), sizeof(int), samples.size(), pFile);

            for (const auto& sample : data)
            {
                fprintf(pFile, "%d\n", sample);
            }
        }

        fclose(pFile);
    }
}

void ToCSV(const std::vector<std::complex<double>>& data, const std::wstring& fileName)
{
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\" << fileName << "FFT.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.str().c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            for (const auto& sample : data)
            {
                fprintf(pFile, "%f,%f\n", sample.real(), sample.imag());
            }
        }

        fclose(pFile);
    }
}

void ToCSV(const std::vector<std::pair<int, int>>& data, const std::wstring& fileName)
{
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\" << fileName << "SortedMag.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.str().c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            for (const auto& sample : data)
            {
                fprintf(pFile, "%d,%d\n", sample.first, sample.second);
            }
        }

        fclose(pFile);
    }
}

void ToMagnitudeCSV(const std::vector<std::complex<double>>& data, const std::wstring& fileName)
{
    std::vector<std::pair<int, int>> magData;
    magData.resize(data.size());

    // Convert to magnitude
    std::transform(data.begin(), data.end(), magData.begin(), [](std::complex<double> dataPoint)
    {
        return std::pair<int, int>(0, std::abs(dataPoint));
    });

    // Add the frequencies
    for (size_t i = 0; i < magData.size(); i++)
    {
        magData[i].first = i;
    }

    // Sort by magnitude
    std::sort(magData.begin(), magData.end(), [](std::pair<int, int> l, std::pair<int, int> r)
    {
        return (l.second > r.second);
    });

    ToCSV(magData, fileName);
}

int main()
{
    // ProcessRecording(L"E", L"2sTo3s", 88200, 132300);
    GenerateMattBass();

    return 0;
}
