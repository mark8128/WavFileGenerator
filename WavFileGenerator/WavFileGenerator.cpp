#include "stdafx.h"

#include <complex>
#include "FileBuilder.h"
#include "Fourier.h"
#include "FurElise.h"
#include "Note.h"
#include "WavReader.h"
#include "WavSamples.h"

void ToCSV(const std::vector<int>&);
void ToCSV(const std::vector<std::complex<double>>&);

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

    // DURATION OF 0.25 leads to the FFT being off bafo 4, .2 bafo 5
    Note concertA(440, 0, 0.25);
    Note middleC(261.6, 0.1, 0.2);

    notes.push_back(concertA);
    /*notes.push_back(middleC);

    notes.push_back(Note(Tone::C, 4, 0.25, 0.05));
    notes.push_back(Note(Tone::D, 4, 0.3, 0.05));
    notes.push_back(Note(Tone::E, 4, 0.35, 0.05));
    notes.push_back(Note(Tone::F, 4, 0.4, 0.05));
    notes.push_back(Note(Tone::G, 4, 0.45, 0.05));
    notes.push_back(Note(Tone::A, 4, 0.5, 0.05));
    notes.push_back(Note(Tone::B, 4, 0.55, 0.05));
    notes.push_back(Note(Tone::C, 5, 0.6, 0.05));*/

    FileBuilder builder(notes);
    builder.ToFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\FFTTestWav3.wav");
}

void ReadWav()
{
    WavReader reader;
    WavSamples samples;

    reader.ReadFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\File1.wav", samples);

    ToCSV(samples._samples[0]);
}

void FFTTest()
{
    GenerateFFTTestWav();

    WavReader reader;
    WavSamples samples;

    reader.ReadFile(L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\FFTTestWav3.wav", samples);

    ToCSV(samples._samples[0]);

    Fourier fourier;
    std::vector<std::complex<double>> fft;

    // Fourier granularity is sample rate / size. Sample rate is 44100 (defined in wav files used in this application)
    // The time of the note is inversely proportional to the granularity, so the axis should be plotted as 1/N.
    fourier.ComputeFFT(samples._samples[0], fft);

    ToCSV(fft);
}

void ToCSV(const std::vector<int>& data)
{
    std::wstring path = L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\Vector3.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.c_str(), L"wb");

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

void ToCSV(const std::vector<std::complex<double>>& data)
{
    std::wstring path = L"C:\\Users\\Mark Mahony\\source\\repos\\WavFileGenerator\\Output\\FFTOutput3.csv";

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, path.c_str(), L"wb");

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

int main()
{
    FFTTest();

    return 0;
}
