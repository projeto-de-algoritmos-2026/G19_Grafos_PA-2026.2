#ifndef FRETBOARD_H
#define FRETBOARD_H

#include <string>
#include <vector>

#ifndef NOTE_POS_DEFINED
#define NOTE_POS_DEFINED
struct NotePos {
    int string;
    int fret;
    int midi;
};
#endif

extern const int OPEN_STRINGS[6];

int noteToMidi(const std::string& name);

std::string midiToNote(int midi);

std::vector<NotePos> positionsForNote(int midi, int maxFret = 12);

#endif