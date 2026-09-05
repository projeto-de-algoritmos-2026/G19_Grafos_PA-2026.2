#ifndef CHORDS_H
#define CHORDS_H

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

extern const std::unordered_map<std::string, std::vector<int>> CHORD_INTERVALS;

std::pair<int, std::vector<int>> parseChord(const std::string& name);
std::vector<int> chordTones(int rootMidi, const std::vector<int>& intervals);

#endif
