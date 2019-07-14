#pragma once

#include <vector>

struct Note;
struct WavSamples;

class WavReader
{
public:
    WavReader();
    ~WavReader() = default;

    void ReadFile(_In_ const std::wstring& path, _Out_ WavSamples& samples);

    void Test();

private:
    void BytesFromFile(_In_ const std::wstring& path, _Out_ std::vector<std::byte>& rawBytes);
    void GetEncodingFormat(
        _In_ std::vector<std::byte>& rawBytes,
        _Out_ WavSamples& samples);
    void GetSamples(
        _In_ std::vector<std::byte>& rawBytes,
        _Inout_ WavSamples& samples);
};