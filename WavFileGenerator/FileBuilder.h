#pragma once
#include <vector>
#include <functional>

struct Note;

class FileBuilder
{
public:
    FileBuilder();
    FileBuilder(std::vector<Note> notes);
    ~FileBuilder() = default;

    void ToBuffer(_Out_ std::vector<std::byte>& buffer);
    void ToFile(std::wstring path);

    void Test();

private:
    static const short BytesPerSample;

    void BuildRIFFChunk(uint32_t totalChunkSize, _Out_ std::vector<std::byte>& buffer);

    void BuildFormatChunk(_Out_ std::vector<std::byte>& buffer);
    void BuildDataChunk(_Out_ std::vector<std::byte>& buffer);
    void GetDataBuffer(_Out_ std::vector<std::byte>& buffer);

    void GenerateConcertA(_Out_ std::vector<std::byte>& buffer);

    void GenerateNotes(_Out_ std::vector<std::byte>& buffer);
    uint32_t GetTotalNumberOfSamples();
    void GetSamplesFromNotes(_In_ const std::function<double(FileBuilder&, double, int)>& sampler,_Out_ std::vector<int>& samples);
    void NormalizeValues(_In_ std::vector<int>& samples, _Out_ std::vector<std::byte>& buffer);

    double SineWaveSample(double freq, int sampleIndex);
    double HarmonicSample(double freq, int sampleIndex);
    double LowHarmonicSample(double freq, int sampleIndex);
    double ManyCloseWaves(double freq, int sampleIndex);
    double RoundedFreqMult(double freq, int sampleIndex);

    uint32_t _sampleRate;
    std::vector<Note> _notes;
};