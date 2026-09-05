#include "chords.h"
#include "fretboard.h"
#include <stdexcept>
#include <cctype>
#include <algorithm>

const std::unordered_map<std::string, std::vector<int>> CHORD_INTERVALS = {
    {"maj7", {0, 4, 7, 11}},
    {"m7",   {0, 3, 7, 10}},
    {"7",    {0, 4, 7, 10}},
    {"m",    {0, 3, 7}},
    {"maj",  {0, 4, 7}}
};

std::pair<int, std::vector<int>> parseChord(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Nome do acorde vazio!");
    }

    size_t pos = 0;
    std::string rootStr = "";
    rootStr += name[pos++];

    if (pos < name.size() && (name[pos] == '#' || name[pos] == 'b')) {
        rootStr += name[pos++];
    }

    rootStr += "4";

    int rootMidi = noteToMidi(rootStr);
    std::string quality = name.substr(pos);

    if (quality.empty()) quality = "maj";

    auto it = CHORD_INTERVALS.find(quality);
    if (it != CHORD_INTERVALS.end()) {
        return {rootMidi, it->second};
    }

    return {rootMidi, {0, 4, 7, 10}};
}

std::vector<int> chordTones(int rootMidi, const std::vector<int>& intervals) {
    std::vector<int> tones;
    for (int interval : intervals) {
        tones.push_back(rootMidi + interval);
    }
    return tones;
}
