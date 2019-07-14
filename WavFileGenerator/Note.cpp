#include "stdafx.h"

#include "Note.h"
#include <math.h>

static const double ConcertA = 440.0; // A4

Note::Note() :
    _freq(440),
    _amp(32760),
    _startTime(0),
    _duration(1)
{
}

Note::Note(double freq, double startTime, double duration) :
    _freq(freq),
    _amp(32760),
    _startTime(startTime),
    _duration(duration)
{
}

Note::Note(Tone t, int octave, double startTime, double duration) :
    _amp(32760),
    _startTime(startTime),
    _duration(duration)
{
    _freq = GetFrequency(t, octave);
}

double Note::GetFrequency(Tone t, int octave) const
{
    // A note's frequency can be determined based on A4 using the following exponentation
    // let n be the number of semitones between A4 and the desired frequency
    // freq = A4 * 2 ^ (n/12)
    int n = SemiTonesFromA4(t, octave);

    return (ConcertA * exp2(n / 12.0));
}

int Note::SemiTonesFromA4(Tone t, int octave) const
{
    // First account for the octave differences
    int semiTones = (octave - 4) * 12;

    // Now account for the tones within the octave
    semiTones += static_cast<short>(t) - static_cast<short>(Tone::A);

    return semiTones;
}
