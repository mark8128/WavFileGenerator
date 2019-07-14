#include "stdafx.h"

#include "FurElise.h"
#include "Metronome.h"
#include "Note.h"

FurElise::FurElise()
{
}

void FurElise::Generate(_Out_ std::vector<Note>& notes)
{
    notes.clear();

    Metronome m(0.5, 1.0, 3);

    double beatLength = 0.5;

    // Note, octave, start time, duration

    // Bar 0
    notes.push_back(Note(Tone::E, 5, m.StartTime(0, 0), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(0, 0.5), beatLength / 2));

    // Bar 1
    notes.push_back(Note(Tone::E, 5, m.StartTime(1, 0), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(1, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(1, 1), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(1, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::D, 5, m.StartTime(1, 2), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(1, 2.5), beatLength / 2));

    // Bar 2
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(2, 0), beatLength));
    notes.push_back(Note(Tone::C, 4, m.StartTime(2, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 4, m.StartTime(2, 2), beatLength / 2));
    notes.push_back(Note(Tone::A, 4, m.StartTime(2, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(2, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(2, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(2, 1), beatLength / 2));

    // Bar 3
    // Treble
    notes.push_back(Note(Tone::B, 4, m.StartTime(3, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(3, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 4, m.StartTime(3, 2), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(3, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::E, 2, m.StartTime(3, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(3, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 3, m.StartTime(3, 1), beatLength / 2));

    // Bar 4
    // Treble
    notes.push_back(Note(Tone::C, 5, m.StartTime(4, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(4, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(4, 2), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(4, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(4, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(4, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(4, 1), beatLength / 2));

    // Bar 5
    notes.push_back(Note(Tone::E, 5, m.StartTime(5, 0), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(5, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(5, 1), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(5, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::D, 5, m.StartTime(5, 2), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(5, 2.5), beatLength / 2));

    // Bar 6
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(6, 0), beatLength));
    notes.push_back(Note(Tone::C, 4, m.StartTime(6, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 4, m.StartTime(6, 2), beatLength / 2));
    notes.push_back(Note(Tone::A, 4, m.StartTime(6, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(6, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(6, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(6, 1), beatLength / 2));

    // Bar 7
    // Treble
    notes.push_back(Note(Tone::B, 4, m.StartTime(7, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(7, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(7, 2), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(7, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::E, 2, m.StartTime(7, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(7, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 3, m.StartTime(7, 1), beatLength / 2));

    // Bar 8
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(8, 0), beatLength));
    notes.push_back(Note(Tone::E, 5, m.StartTime(8, 2), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(8, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(8, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(8, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(8, 1), beatLength / 2));

    // Back to bar 1

    // Bar 9
    notes.push_back(Note(Tone::E, 5, m.StartTime(9, 0), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(9, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(9, 1), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(9, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::D, 5, m.StartTime(9, 2), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(9, 2.5), beatLength / 2));

    // Bar 10
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(10, 0), beatLength));
    notes.push_back(Note(Tone::C, 4, m.StartTime(10, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 4, m.StartTime(10, 2), beatLength / 2));
    notes.push_back(Note(Tone::A, 4, m.StartTime(10, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(10, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(10, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(10, 1), beatLength / 2));

    // Bar 11
    // Treble
    notes.push_back(Note(Tone::B, 4, m.StartTime(11, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(11, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 4, m.StartTime(11, 2), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(11, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::E, 2, m.StartTime(11, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(11, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 3, m.StartTime(11, 1), beatLength / 2));

    // Bar 12
    // Treble
    notes.push_back(Note(Tone::C, 5, m.StartTime(12, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(12, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(12, 2), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(12, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(12, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(12, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(12, 1), beatLength / 2));

    // Bar 13
    notes.push_back(Note(Tone::E, 5, m.StartTime(13, 0), beatLength / 2));
    notes.push_back(Note(Tone::DSharp, 5, m.StartTime(13, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 5, m.StartTime(13, 1), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(13, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::D, 5, m.StartTime(13, 2), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(13, 2.5), beatLength / 2));

    // Bar 14
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(14, 0), beatLength));
    notes.push_back(Note(Tone::C, 4, m.StartTime(14, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::E, 4, m.StartTime(14, 2), beatLength / 2));
    notes.push_back(Note(Tone::A, 4, m.StartTime(14, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(14, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(14, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(14, 1), beatLength / 2));

    // Bar 15
    // Treble
    notes.push_back(Note(Tone::B, 4, m.StartTime(15, 0), beatLength));
    notes.push_back(Note(Tone::E, 4, m.StartTime(15, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(15, 2), beatLength / 2));
    notes.push_back(Note(Tone::B, 4, m.StartTime(15, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::E, 2, m.StartTime(15, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(15, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::GSharp, 3, m.StartTime(15, 1), beatLength / 2));

    // Bar 16
    // Treble
    notes.push_back(Note(Tone::A, 4, m.StartTime(16, 0), beatLength));
    notes.push_back(Note(Tone::B, 4, m.StartTime(16, 1.5), beatLength / 2));
    notes.push_back(Note(Tone::C, 5, m.StartTime(16, 2), beatLength / 2));
    notes.push_back(Note(Tone::D, 5, m.StartTime(16, 2.5), beatLength / 2));

    // Bass
    notes.push_back(Note(Tone::A, 2, m.StartTime(16, 0), beatLength / 2));
    notes.push_back(Note(Tone::E, 3, m.StartTime(16, 0.5), beatLength / 2));
    notes.push_back(Note(Tone::A, 3, m.StartTime(16, 1), beatLength / 2));

    // Entering next repeat section

    // Bar 17 to 30 is performed twice
    for (int i = 0; i < 2; i++)
    {
        // Bar 17 + 31
        // Treble
        notes.push_back(Note(Tone::E, 5, m.StartTime(17 + i * 14, 0), 3 * beatLength / 2));
        notes.push_back(Note(Tone::G, 4, m.StartTime(17 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::F, 5, m.StartTime(17 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(17 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::C, 3, m.StartTime(17 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::G, 3, m.StartTime(17 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::C, 4, m.StartTime(17 + i * 14, 1), beatLength / 2));

        // Bar 18 + 32
        // Treble
        notes.push_back(Note(Tone::D, 5, m.StartTime(18 + i * 14, 0), 3 * beatLength / 2));
        notes.push_back(Note(Tone::F, 4, m.StartTime(18 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(18 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::D, 5, m.StartTime(18 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::G, 2, m.StartTime(18 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::G, 3, m.StartTime(18 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::B, 3, m.StartTime(18 + i * 14, 1), beatLength / 2));

        // Bar 19 + 33
        // Treble
        notes.push_back(Note(Tone::C, 5, m.StartTime(19 + i * 14, 0), 3 * beatLength / 2));
        notes.push_back(Note(Tone::E, 4, m.StartTime(19 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::D, 5, m.StartTime(19 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::C, 5, m.StartTime(19 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::A, 2, m.StartTime(19 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(19 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::A, 3, m.StartTime(19 + i * 14, 1), beatLength / 2));

        // Bar 20 + 34
        // Treble
        notes.push_back(Note(Tone::B, 4, m.StartTime(20 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::E, 4, m.StartTime(20 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(20 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::E, 4, m.StartTime(20 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::E, 2, m.StartTime(20 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(20 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 4, m.StartTime(20 + i * 14, 1), beatLength / 2));

        // Bar 21 + 35
        // Treble
        notes.push_back(Note(Tone::E, 5, m.StartTime(21 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(21 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 6, m.StartTime(21 + i * 14, 1), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(21 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(21 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(21 + i * 14, 2.5), beatLength / 2));

        // Bar 22 + 36
        notes.push_back(Note(Tone::E, 5, m.StartTime(22 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(22 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(22 + i * 14, 1), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(22 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(22 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(22 + i * 14, 2.5), beatLength / 2));

        // Bar 23 + 37
        // Treble
        notes.push_back(Note(Tone::E, 5, m.StartTime(23 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(23 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(23 + i * 14, 1), beatLength / 2));
        notes.push_back(Note(Tone::B, 4, m.StartTime(23 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::D, 5, m.StartTime(23 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::C, 5, m.StartTime(23 + i * 14, 2.5), beatLength / 2));

        // Bar 24 + 38
        // Treble
        notes.push_back(Note(Tone::A, 4, m.StartTime(24 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::C, 4, m.StartTime(24 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 4, m.StartTime(24 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::A, 4, m.StartTime(24 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::A, 2, m.StartTime(24 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(24 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::A, 3, m.StartTime(24 + i * 14, 1), beatLength / 2));

        // Bar 25 + 39
        // Treble
        notes.push_back(Note(Tone::B, 4, m.StartTime(25 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::E, 4, m.StartTime(25 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::GSharp, 4, m.StartTime(25 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::B, 4, m.StartTime(25 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::E, 2, m.StartTime(25 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(25 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::GSharp, 3, m.StartTime(25 + i * 14, 1), beatLength / 2));

        // Bar 26 + 40
        // Treble
        notes.push_back(Note(Tone::C, 5, m.StartTime(26 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::E, 4, m.StartTime(26 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(26 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(26 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::A, 2, m.StartTime(26 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(26 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::A, 3, m.StartTime(26 + i * 14, 1), beatLength / 2));

        // Bar 27 + 41
        // Treble
        notes.push_back(Note(Tone::E, 5, m.StartTime(27 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::DSharp, 5, m.StartTime(27 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 5, m.StartTime(27 + i * 14, 1), beatLength / 2));
        notes.push_back(Note(Tone::B, 4, m.StartTime(27 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::D, 5, m.StartTime(27 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::C, 5, m.StartTime(27 + i * 14, 2.5), beatLength / 2));

        // Bar 28 + 42
        // Treble
        notes.push_back(Note(Tone::A, 4, m.StartTime(28 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::C, 4, m.StartTime(28 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::E, 4, m.StartTime(28 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::A, 4, m.StartTime(28 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::A, 2, m.StartTime(28 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(28 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::A, 3, m.StartTime(28 + i * 14, 1), beatLength / 2));

        // Bar 29 + 43
        // Treble
        notes.push_back(Note(Tone::B, 4, m.StartTime(29 + i * 14, 0), beatLength));
        notes.push_back(Note(Tone::E, 4, m.StartTime(29 + i * 14, 1.5), beatLength / 2));
        notes.push_back(Note(Tone::C, 5, m.StartTime(29 + i * 14, 2), beatLength / 2));
        notes.push_back(Note(Tone::B, 4, m.StartTime(29 + i * 14, 2.5), beatLength / 2));

        // Bass
        notes.push_back(Note(Tone::E, 2, m.StartTime(29 + i * 14, 0), beatLength / 2));
        notes.push_back(Note(Tone::E, 3, m.StartTime(29 + i * 14, 0.5), beatLength / 2));
        notes.push_back(Note(Tone::GSharp, 3, m.StartTime(29 + i * 14, 1), beatLength / 2));

        // Bar 30
        if (i == 0)
        {
            // Treble
            notes.push_back(Note(Tone::A, 4, m.StartTime(30, 0), beatLength));
            notes.push_back(Note(Tone::B, 4, m.StartTime(30, 1.5), beatLength / 2));
            notes.push_back(Note(Tone::C, 5, m.StartTime(30, 2), beatLength / 2));
            notes.push_back(Note(Tone::D, 5, m.StartTime(30, 2.5), beatLength / 2));

            // Bass
            notes.push_back(Note(Tone::A, 2, m.StartTime(30, 0), beatLength / 2));
            notes.push_back(Note(Tone::E, 3, m.StartTime(30, 0.5), beatLength / 2));
            notes.push_back(Note(Tone::A, 3, m.StartTime(30, 1), beatLength / 2));
        }
    }

    // Ending note
    notes.push_back(Note(Tone::A, 4, m.StartTime(44, 0), beatLength));

    notes.push_back(Note(Tone::A, 4, m.StartTime(45, 0), 0.1));

    //notes.push_back(Note(Tone::C, 5, m.StartTime(45, 0), 12 * beatLength));
}