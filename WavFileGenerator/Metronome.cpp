#include "stdafx.h"
#include "Metronome.h"

Metronome::Metronome() :
    _beatDuration(0.5),
    _offset(0.0),
    _beatsInABar(4)
{
}

Metronome::Metronome(double beatDuration, double beatsInZerothBar, int beatsInABar) :
    _beatDuration(beatDuration),
    _offset(beatsInZerothBar * beatDuration),
    _beatsInABar(beatsInABar)
{
}

// 0 based counting
double Metronome::StartTime(int bar, double beat)
{
    double startTime = 0.0;

    if (bar == 0)
    {
        startTime = (beat * _beatDuration);
    }
    else
    {
        // Account for the zeroth bar's partial time signature
        startTime += _offset;

        // Account for the full bars that have past
        startTime += (bar - 1) * _beatsInABar * _beatDuration;

        // Account for the indexing into the current bar
        startTime += (beat * _beatDuration);
    }

    return startTime;
}
