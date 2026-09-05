#ifndef CHORDS_H
#define CHORDS_H

#include <string>
#include <vector>
#include <unordered_map>

// Mapa de intervalos por tipo de acorde (em semitons)
extern const std::unordered_map<std::string, std::vector<int>> CHORD_INTERVALS;

// Parse de acorde: retorna (rootMidi, intervals)
std::pair<int, std::vector<int>> parseChord(const std::string& name);

// Gera as notas do acorde (MIDI) a partir da fundamental e dos intervalos
std::vector<int> chordTones(int rootMidi, const std::vector<int>& intervals);

#endif // CHORDS_H