#pragma once

// What defines a note:
// Start time
// Duration
// Frequency
// Volume

enum class Tone : short
{
    C = 0,
    CSharp = 1,
    D = 2,
    DSharp = 3,
    E = 4,
    F = 5,
    FSharp = 6,
    G = 7,
    GSharp = 8,
    A = 9,
    ASharp = 10,
    B = 11,
    Octave = 12,
};

struct Note
{
    Note();
    Note(double freq, double startTime, double duration);
    Note(Tone t, int octave, double startTime, double duration);

    ~Note() = default;

    double _freq;
    short _amp;
    double _startTime;
    double _duration;

private:
    double GetFrequency(Tone t, int octave) const;
    int SemiTonesFromA4(Tone t, int octave) const;
};