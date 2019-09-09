#pragma once
#include <complex>
#include <string>
#include <vector>

struct AnalysisDataEntry;

struct AnalysisData
{
    AnalysisData();

    void WriteBaseFrequency(FILE* pFile) const;
    void WriteHeader(FILE* pFile) const;
    void WriteDataEntries(FILE* pFile) const;

    int baseFreq;
    std::vector<int> startingSamples;
    std::vector<AnalysisDataEntry> dataEntries;
};

struct AnalysisDataEntry
{
    AnalysisDataEntry();

    void WriteToFile(FILE* pFile) const;

    int freq;
    std::vector<int> fftValues;
    int sum;
    int harmonic;
};

class RecordingAnalyzer
{
public:
    RecordingAnalyzer();
    ~RecordingAnalyzer() = default;

    void AnalyzeRecording(const std::wstring& fileName);
    void AnalyzeRecording(const std::wstring& fileName, int sampleInterval, int intervalLength);
    void ToCSV(const std::wstring& fileName);

private:
    typedef std::vector<std::complex<double>> FFTData;

    void ComputeFFTsOnInterval(
        const std::vector<int>& samples,
        int interval,
        int intervalLegnth,
        _Out_ std::map<int, FFTData>& ffts);

    void ProcessFFTData(const std::map<int, FFTData>& ffts);

    void PopulateStartingSamples(const std::map<int, FFTData>& ffts);
    void PopulateDataEntriesNoHarmonic(const std::map<int, FFTData>& ffts);
    void ComputeBaseFrequency();
    void PopulateDataEntryHarmonics();

    AnalysisData _analysisData;
};