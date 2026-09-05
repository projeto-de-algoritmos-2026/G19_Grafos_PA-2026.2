#include "fretboard.h"
#include <unordered_map>
#include <cctype>
#include <stdexcept>
#include <algorithm>

const int OPEN_STRINGS[6] = {40, 45, 50, 55, 59, 64};

int noteToMidi(const std::string& name) {
    static const std::unordered_map<char, int> NOTE_TO_SEMITONE = {
        {'C', 0}, {'D', 2}, {'E', 4}, {'F', 5}, {'G', 7}, {'A', 9}, {'B', 11}
    };

    if (name.empty()) {
        throw std::invalid_argument("Nome da nota vazio!");
    }

    size_t pos = 0;
    char noteChar = (char)std::toupper(name[pos++]);

    auto it = NOTE_TO_SEMITONE.find(noteChar);
    if (it == NOTE_TO_SEMITONE.end()) {
        throw std::invalid_argument("Nota invalida!");
    }

    int basePitch = it->second;

    if (pos < name.size() && (name[pos] == '#' || name[pos] == 'b')) {
        if (name[pos] == '#') basePitch++;
        else if (name[pos] == 'b') basePitch--;
        pos++;
    }

    int octave = 4;
    if (pos < name.size()) {
        octave = std::stoi(name.substr(pos));
    }

    return (octave + 1) * 12 + basePitch;
}

std::string midiToNote(int midi) {
    static const std::vector<std::string> NOTE_NAMES = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };
    int noteIdx = midi % 12;
    int octave = (midi / 12) - 1;
    return NOTE_NAMES[noteIdx] + std::to_string(octave);
}

std::vector<NotePos> positionsForNote(int midi, int maxFret) {
    std::vector<NotePos> positions;
    for (int str = 0; str < 6; ++str) {
        int openMidi = OPEN_STRINGS[str];
        int fret = midi - openMidi;
        if (fret >= 0 && fret <= maxFret) {
            positions.push_back({str, fret, midi});
        }
    }
    return positions;
}