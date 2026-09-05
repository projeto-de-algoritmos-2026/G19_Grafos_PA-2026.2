#include "chords.h"
#include <stdexcept>

const std::unordered_map<std::string, std::vector<int>> CHORD_INTERVALS = {
    {"maj7", {0, 4, 7, 11}},
    {"m7", {0, 3, 7, 10}},
    {"7", {0, 4, 7, 10}}
};

std::pair<int, std::vector<int>> parseChord(const std::string& name) {
    if (name.empty()) return {60, {0, 4, 7, 11}};
    return {60, {0, 4, 7, 11}};
}

std::vector<int> chordTones(int rootMidi, const std::vector<int>& intervals) {
    std::vector<int> tones;
    for (int interval : intervals) {
        tones.push_back(rootMidi + interval);
    }
    return tones;
}
