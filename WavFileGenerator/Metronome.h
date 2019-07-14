#pragma once

class Metronome
{
public:
    Metronome();
    Metronome(double beatDuration, double beatsInZerothBar, int beatsInABar);

    double StartTime(int bar, double beat);

private:
    double _beatDuration;
    double _offset;
    int _beatsInABar;
};