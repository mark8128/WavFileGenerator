#include "stdafx.h"

#include <algorithm>
#include <complex>
#include "FileBuilder.h"
#include "Fourier.h"
#include "FurElise.h"
#include <map>
#include "Note.h"
#include "RecordingAnalyzer.h"
#include "WavReader.h"
#include "WavSamples.h"

void ToCSV(const std::vector<int>&, const std::wstring&);
void ToCSV(const std::vector<std::complex<double>>&, const std::wstring&);
void ToMultiFFTCSV(
    const std::vector<std::vector<std::pair<int, int>>>& data,
    const std::vector<int>& startingSamples,
    const std::wstring& fileName);
void FFTToMagnitude(const std::vector<std::complex<double>>&, _Out_ std::vector<std::pair<int, int>>& magData);

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
        8,              // Only compute the transform for the first 8th of the frequency domain
        startSample,    // Start the FFT on this sample
        endSample,      // Only proccess to this sample
        fft);

    std::wostringstream outputFileName;
    outputFileName << fileName << outputUniquifier;
    ToCSV(fft, outputFileName.str());

    std::vector<std::pair<int, int>> magData;
    FFTToMagnitude(fft, magData);
    //ToCSV(magData, outputFileName.str());
}

void ComputeFFTsOnInterval(
    const std::vector<int>& samples,
    int interval,
    int intervalLegnth,
    _Out_ std::map<int, std::vector<std::complex<double>>>& ffts)
{
    Fourier fourier;
    std::vector<std::complex<double>> fft;

    const int maxSample = samples.size();

    for (int startSample = 0; (startSample + intervalLegnth) <= maxSample; startSample += interval)
    {
        fft.clear();

        fourier.ComputeMultiThreadedFFTForRange(
            samples,
            20,             // Only compute the first 5% of the transform
            startSample,    // Start the FFT on this sample
            (startSample + intervalLegnth),      // Only proccess to this sample
            fft);

        // start sample to fourier transform
        ffts[startSample] = fft;
    }
}

void RollingTransformAnalysis(const std::wstring& fileName, const std::wstring& outputUniquifier)
{
    WavReader reader;
    WavSamples samples;

    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\BassRecording\\" << fileName << ".wav";

    reader.ReadFile(path.str().c_str(), samples);

    std::map<int, std::vector<std::complex<double>>> ffts;
    ComputeFFTsOnInterval(samples._samples[0], 44100 /* interval */, 44100 /* interval length */, ffts);

    // The output is a vector of vectors. The outer vector is the same size as the number of FFTs performed
    // The inner vector contains the magnitude output of each FFT. The first element in the pair in the inner
    // vector is the frequency, the second value is the magnitude.
    std::vector<std::vector<std::pair<int, int>>> output;
    std::vector<int> startingSamples;
    output.resize(ffts.size());
    size_t index = 0;

    for (const auto& fft : ffts)
    {
        startingSamples.push_back(fft.first);

        FFTToMagnitude(fft.second, output[index]);
        index++;
    }

    std::wostringstream outputFileName;
    outputFileName << fileName << outputUniquifier;

    ToMultiFFTCSV(output, startingSamples, outputFileName.str());
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

void ToMultiFFTCSV(
    const std::vector<std::vector<std::pair<int, int>>>& data,
    const std::vector<int>& startingSamples,
    const std::wstring& fileName)
{
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\" << fileName << ".csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.str().c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            // Iterate over the vector of vectors to form the following table:
            // Freq, mag of first fft, mag of 2nd fft, ...
            std::stringstream csvRow;
            int fftSum = 0;

            // Add a column header for the frequencies
            csvRow << "f";

            // Add the starting samples as column headers
            for (const auto& sample : startingSamples)
            {
                csvRow << "," << sample;
            }

            // Add the header for the sum column
            csvRow << ",,sum\n";

            // Flush it to the file
            fprintf(pFile, csvRow.str().c_str());
            csvRow.str(std::string());

            for (int freqIndex = 0; freqIndex < data[0].size(); freqIndex++)
            {
                csvRow.str(std::string());
                fftSum = 0;

                // Push the frequency
                csvRow << data[0][freqIndex].first << ",";

                // Push the data from each FFT for this frequncy
                for (int fftIndex = 0; fftIndex < data.size(); fftIndex++)
                {
                    const int magData = data[fftIndex][freqIndex].second;
                    
                    fftSum += magData;
                    csvRow << magData << ",";
                }

                // Push the sum across the FFTs after a space
                csvRow << "," << fftSum << "\n";

                fprintf(pFile, csvRow.str().c_str());
            }
        }

        fclose(pFile);
    }
}

void FFTToMagnitude(const std::vector<std::complex<double>>& data, _Out_ std::vector<std::pair<int, int>>& magData)
{
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
}

int main()
{
    // ProcessRecording(L"E", L"2sTo3s", 88200, 132300);
    // GenerateMattBass();

    // RollingTransformAnalysis(L"E", L"RollingTransformTable");

    RecordingAnalyzer analyzer;
    analyzer.AnalyzeRecording(L"E");
    analyzer.ToCSV(L"E.RollingTransformTable");

    return 0;
}
