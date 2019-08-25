#pragma once
#include <vector>
#include <functional>

struct Note;

class FileBuilder
{
public:
    enum class SoundStyle
    {
        SineWave = 0,
        HarmonicSine = 1,
        LowHarmonicSine = 2,
        BlurredWaves = 3,
        RoundedFrequencies = 4,
        MattBassEFirstSecondBlendedHarmonics = 5,
        MattBassEFirstSecondPureHarmonics = 6,
        MattBassE2s3sPure = 7,
    };

    FileBuilder();
    FileBuilder(std::vector<Note> notes);
    FileBuilder(std::vector<Note> notes, SoundStyle style);
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

    void GenerateNotes(_Out_ std::vector<std::byte>& buffer);
    uint32_t GetTotalNumberOfSamples();
    void GetSamplesFromNotes(_In_ const std::function<double(FileBuilder&, double, int)>& sampler,_Out_ std::vector<int>& samples);
    void NormalizeValues(_In_ std::vector<int>& samples, _Out_ std::vector<std::byte>& buffer);

    double SineWaveSample(double freq, int sampleIndex);
    double HarmonicSample(double freq, int sampleIndex);
    double LowHarmonicSample(double freq, int sampleIndex);
    double ManyCloseWaves(double freq, int sampleIndex);
    double RoundedFreqMult(double freq, int sampleIndex);
    double MattBassPureHarmonics(double freq, int sampleIndex);
    double MattBassBlendedHarmonics(double freq, int sampleIndex);
    double MattBass2sTo3sPure(double freq, int sampleIndex);

    static const std::unordered_map<SoundStyle, const std::function<double(FileBuilder&, double, int)>>& SampleFunctions;

    uint32_t _sampleRate;
    std::vector<Note> _notes;
    SoundStyle _style;
};