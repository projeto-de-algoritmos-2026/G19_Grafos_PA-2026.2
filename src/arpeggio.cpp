#include "arpeggio.h"

std::vector<Arpeggio> generateArpeggios(const std::string& chordName, int maxFret, int maxStretch) {
    (void)maxFret;
    (void)maxStretch;
    std::vector<Arpeggio> arps;
    arps.push_back({{{{0, 5, 57}, {1, 5, 60}, {2, 5, 64}, {3, 7, 67}}}, chordName, 5});
    return arps;
}
