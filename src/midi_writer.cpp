#include "midi_writer.h"

MidiWriter::MidiWriter(int bpm) {
    midiFile.setTicksPerQuarterNote(480);

    // track, tick, tempo em BPM
    midiFile.addTempo(0, 0, bpm);

    // track, tick, numerador, denominador, clocks por metrônomo,
    // quantidade de semicolcheias por semínima
    midiFile.addTimeSignature(0, 0, 4, 4, 24, 8);

    // Instrumento MIDI 30 = guitarra elétrica clean.
    // track, tick, canal, instrumento
    midiFile.addPatchChange(0, 0, 0, 30);
}

void MidiWriter::addNote(int pitch, int start, int duration, int velocity) {
    // Garante valores MIDI válidos.
    if (pitch < 0 || pitch > 127) {
        return;
    }

    if (velocity < 0) {
        velocity = 0;
    }

    if (velocity > 127) {
        velocity = 127;
    }

    if (duration <= 0) {
        return;
    }

    midiFile.addNoteOn(0, start, 0, pitch, velocity);
    midiFile.addNoteOff(0, start + duration, 0, pitch);
}

void MidiWriter::save(const std::string& filename) {
    midiFile.sortTracks();
    midiFile.write(filename);
}