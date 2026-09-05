#ifndef ARPEGGIO_H
#define ARPEGGIO_H

#include "fretboard.h"
#include <string>
#include <vector>

#ifndef ARPEGGIO_DEFINED
#define ARPEGGIO_DEFINED
struct Arpeggio {
    std::vector<NotePos> notes;
    std::string chordName;
    int regionFret;
};
#endif

std::vector<Arpeggio> generateArpeggios(const std::string& chordName, int maxFret = 12, int maxStretch = 4);

void printArpeggioTab(const std::vector<Arpeggio>& arpeggios, const std::string& title="");

#endif
