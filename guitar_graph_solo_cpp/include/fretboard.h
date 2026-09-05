#ifndef FRETBOARD_H
#define FRETBOARD_H

#include <string>
#include <vector>

struct NotePos{
    int string; // 0 (E grave) a 5 (E aguda)
    int fret; // casa: 0 a 12
    int midi; // valor MIDI da nota
};

// array de int de tamanho 6 global
// afinação padrão: E2, A2, D3, G3, B3, E4
extern const int OPEN_STRINGS[6];

// função de conversão de nota para MIDI
int noteToMidi(const std::string& name);

// converte MIDI para nota
std::string midiToNote(int midi);

std::vector<NotePos> positionsForNote(int midi, int maxFret = 12);

#endif