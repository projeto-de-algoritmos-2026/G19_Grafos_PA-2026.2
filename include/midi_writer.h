#ifndef MIDI_WRITER_H
#define MIDI_WRITER_H

#include "MidiFile.h"
#include <vector>
#include <string>

struct Note {
    int pitch;      // Valor MIDI (60 = C4)
    int start;      // Tick de início
    int duration;   // Duração em ticks
    int velocity;   // Intensidade (0-127)
};

class MidiWriter {
public:
    MidiWriter(int bpm = 120); 
    
    void addNote(int pitch, int start, int duration, int velocity = 100);
    void save(const std::string& filename);
    
private:
    smf::MidiFile midiFile;
    int tempo;
};

#endif