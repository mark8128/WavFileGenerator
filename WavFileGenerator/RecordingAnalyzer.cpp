#include "stdafx.h"

#include <algorithm>
#include "Fourier.h"
#include <map>
#include "RecordingAnalyzer.h"
#include "WavReader.h"
#include "WavSamples.h"

AnalysisData::AnalysisData() :
    baseFreq(0)
{
}

void AnalysisData::WriteBaseFrequency(FILE* pFile) const
{
    std::stringstream csvRow;
    csvRow << "Base Freq:," << baseFreq << "\n";

    // Flush it to the file
    fprintf(pFile, csvRow.str().c_str());
}

void AnalysisData::WriteHeader(FILE* pFile) const
{
    std::stringstream csvRow;

    // f, sample1, sample2, ..., sampleN, <empty>, sum, harmonic
    csvRow << "Freq,";

    // Add all the starting samples
    for (const auto& sample : startingSamples)
    {
        csvRow << sample << ",";
    }

    // Add the sum and harmonic data
    csvRow << ",Sum,Harmonic\n";

    // Flush it to the file
    fprintf(pFile, csvRow.str().c_str());
}

void AnalysisData::WriteDataEntries(FILE* pFile) const
{
    for (const auto& entry : dataEntries)
    {
        entry.WriteToFile(pFile);
    }
}

AnalysisDataEntry::AnalysisDataEntry() :
    freq(0),
    sum(0),
    harmonic(0)
{
}

void AnalysisDataEntry::WriteToFile(FILE* pFile) const
{
    std::stringstream csvRow;

    // Push the frequency
    csvRow << freq << ",";

    // Push the values for each fft contained in the fftValues collection
    for (const auto& value : fftValues)
    {
        csvRow << value << ",";
    }

    // Push the sum and harmonic data
    csvRow << "," << sum << "," << harmonic << "\n";

    fprintf(pFile, csvRow.str().c_str());
}

RecordingAnalyzer::RecordingAnalyzer()
{
}

void RecordingAnalyzer::AnalyzeRecording(const std::wstring& fileName)
{
    // Perform 1s long FFTs on a 1s interval
    AnalyzeRecording(fileName, 44100, 44100);
}

void RecordingAnalyzer::AnalyzeRecording(const std::wstring& fileName, int sampleInterval, int intervalLength)
{
    WavReader reader;
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\BassRecording\\" << fileName << ".wav";

    WavSamples rawSamples;
    reader.ReadFile(path.str().c_str(), rawSamples);
    
    // Map from starting sample to FFT data
    std::map<int, FFTData> ffts;

    ComputeFFTsOnInterval(rawSamples._samples[0], sampleInterval, intervalLength, ffts);

    ProcessFFTData(ffts);
}

void RecordingAnalyzer::ComputeFFTsOnInterval(
    const std::vector<int>& samples,
    int interval,
    int intervalLegnth,
    _Out_ std::map<int, FFTData>& ffts)
{
    Fourier fourier;
    FFTData fft;

    const int maxSample = samples.size();

    // Always start at the beginning of the recording. Using the provided interval and length
    // compute as many complete FFTs as will fit within the given sample data
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

void RecordingAnalyzer::ProcessFFTData(const std::map<int, FFTData>& ffts)
{
    PopulateStartingSamples(ffts);

    PopulateDataEntriesNoHarmonic(ffts);

    ComputeBaseFrequency();

    PopulateDataEntryHarmonics();
}

void RecordingAnalyzer::PopulateStartingSamples(const std::map<int, FFTData>& ffts)
{
    // Populate the list of starting samples in the AnalysisData
    for (const auto& fft : ffts)
    {
        _analysisData.startingSamples.push_back(fft.first);
    }
}

void RecordingAnalyzer::PopulateDataEntriesNoHarmonic(const std::map<int, FFTData>& ffts)
{
    // Pre-reserve the AnalysisDataEntries so the data can be inserted directly into the
    // vector within the AnalysisData
    const size_t highestFreq = (*ffts.begin()).second.size();
    _analysisData.dataEntries.resize(highestFreq);

    // Build the list of AnalysisDataEntries by iterating through all the frequencies
    // (Assumption: the FFT was performed at a granularlity where the buckets are 1hz)
    for (size_t freq = 0; freq < highestFreq; freq++)
    {
        AnalysisDataEntry* pEntry = &_analysisData.dataEntries[freq];

        // Fill out all the data except for the harmonic
        int sum = 0;
        pEntry->freq = freq;

        // Iterate through the list of FFTs provided and get the complex number
        // generated for the current frequency. Compute its magnitude and add it to the list
        for (const auto& fft : ffts)
        {
            const int magnitude = static_cast<int>(std::abs(fft.second[freq]));
            pEntry->fftValues.push_back(magnitude);
            sum += magnitude;
        }

        pEntry->sum = sum;
    }

    // Sort the AnalysisDataEntries by the sum across the intervals
    std::sort(
        _analysisData.dataEntries.begin(),
        _analysisData.dataEntries.end(),
        [](AnalysisDataEntry left, AnalysisDataEntry right)
    {
        // Sort from biggest to smallest by sum
        return left.sum > right.sum;
    });
}

void RecordingAnalyzer::ComputeBaseFrequency()
{
    // Determine the base frequency
    const auto minElement = std::min_element(
        _analysisData.dataEntries.cbegin(),
        _analysisData.dataEntries.cbegin() + 10, // Look at the first 10 elements (10 largest by sum)
        [](AnalysisDataEntry left, AnalysisDataEntry right)
    {
        return left.freq < right.freq;
    });

    _analysisData.baseFreq = minElement->freq;
}

void RecordingAnalyzer::PopulateDataEntryHarmonics()
{
    for (auto& entry : _analysisData.dataEntries)
    {
        const double harmonicRatio = (static_cast<double>(entry.freq) / _analysisData.baseFreq);
        entry.harmonic = static_cast<int>(std::round(harmonicRatio));
    }
}

void RecordingAnalyzer::ToCSV(const std::wstring& fileName)
{
    std::wostringstream path;
    path << L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\" << fileName << ".csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.str().c_str(), L"wb");

    if (pFile != nullptr)
    {
        if (err == 0)
        {
            _analysisData.WriteBaseFrequency(pFile);
            _analysisData.WriteHeader(pFile);
            _analysisData.WriteDataEntries(pFile);
        }

        fclose(pFile);
    }
}