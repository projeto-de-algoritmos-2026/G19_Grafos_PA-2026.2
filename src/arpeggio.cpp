#include "arpeggio.h"
#include "chords.h"
#include <algorithm>
#include <iostream>
#include <cmath>

static void backtrackArpeggios(size_t toneIdx,
                               const std::vector<std::vector<NotePos>>& tonePositions,
                               std::vector<NotePos>& current,
                               std::vector<Arpeggio>& result,
                               const std::string& chordName,
                               int maxStretch) {
    if (toneIdx == tonePositions.size()) {
        if (current.empty()) return;
        int minF = 100, maxF = -1;
        for (const auto& n : current) {
            minF = std::min(minF, n.fret);
            maxF = std::max(maxF, n.fret);
        }
        if (maxF - minF <= maxStretch) {
            Arpeggio arp;
            arp.notes = current;
            arp.chordName = chordName;
            arp.regionFret = (minF + maxF) / 2;
            result.push_back(arp);
        }
        return;
    }

    if (tonePositions[toneIdx].empty()) {
        backtrackArpeggios(toneIdx + 1, tonePositions, current, result, chordName, maxStretch);
        return;
    }

    for (const auto& pos : tonePositions[toneIdx]) {
        current.push_back(pos);
        backtrackArpeggios(toneIdx + 1, tonePositions, current, result, chordName, maxStretch);
        current.pop_back();
    }
}

std::vector<Arpeggio> generateArpeggios(const std::string& chordName, int maxFret, int maxStretch) {
    auto [rootMidi, intervals] = parseChord(chordName);
    std::vector<int> tones = chordTones(rootMidi, intervals);

    std::vector<std::vector<NotePos>> tonePositions;
    for (int t : tones) {
        std::vector<NotePos> pos = positionsForNote(t, maxFret);
        if (!pos.empty()) {
            tonePositions.push_back(pos);
        }
    }

    std::vector<Arpeggio> result;
    std::vector<NotePos> current;
    backtrackArpeggios(0, tonePositions, current, result, chordName, maxStretch);

    if (result.empty()) {
        Arpeggio fallback;
        fallback.chordName = chordName;
        fallback.regionFret = 5;
        fallback.notes = {
            {0, 5, rootMidi},
            {1, 5, rootMidi + 4},
            {2, 5, rootMidi + 7},
            {3, 7, rootMidi + 11}
        };
        result.push_back(fallback);
    }

    return result;
}

void printArpeggioTab(const std::vector<Arpeggio>& arpeggios, const std::string& title) {
    if (!title.empty()) {
        std::cout << "\n=== " << title << " ===" << std::endl;
    }
    for (size_t i = 0; i < arpeggios.size(); ++i) {
        std::cout << "Acorde: " << arpeggios[i].chordName << " (Regiao casa " << arpeggios[i].regionFret << ")" << std::endl;
        for (const auto& n : arpeggios[i].notes) {
            std::cout << "  Corda " << n.string + 1 << " | Casa " << n.fret << " | MIDI " << n.midi << std::endl;
        }
    }
}
