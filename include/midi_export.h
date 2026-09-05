#ifndef MIDI_EXPORT_H
#define MIDI_EXPORT_H

#include "arpeggio.h"
#include <vector>
#include <string>

void exportMidi(const std::vector<Arpeggio>& solo, const std::string& filename, int bpm = 100);

#endif
