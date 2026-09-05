#include "midi_export.h"
#include "midi_writer.h"

#include <algorithm>
#include <vector>

void exportMidi(const std::vector<Arpeggio>& solo,
                const std::string& filename,
                int bpm) {
    MidiWriter writer(bpm);

    int currentTick = 0;

    constexpr int semicolcheia = 120;
    constexpr int colcheia = 240;
    constexpr int seminima = 480;
    constexpr int minima = 960;

    auto tocarNota = [&](int midi, int duration, int velocity) {
        if (midi < 0 || midi > 127) {
            return;
        }

        writer.addNote(midi, currentTick, duration, velocity);
        currentTick += duration;
    };

    for (size_t i = 0; i < solo.size(); ++i) {
        const Arpeggio& arpeggio = solo[i];

        if (arpeggio.notes.empty()) {
            continue;
        }

        // Extrai as notas MIDI e organiza da mais grave à mais aguda.
        std::vector<int> notes;
        for (const NotePos& note : arpeggio.notes) {
            notes.push_back(note.midi);
        }

        std::sort(notes.begin(), notes.end());

        int tonic = notes[0];
        int third = notes.size() > 1 ? notes[1] : tonic;
        int fifth = notes.size() > 2 ? notes[2] : notes.back();
        int seventh = notes.size() > 3 ? notes[3] : notes.back();

        // Aproximação cromática: cria tensão antes da nota do acorde.
        tocarNota(tonic - 1, semicolcheia, 65);
        tocarNota(tonic, colcheia, 105);

        // Subida do arpejo.
        tocarNota(third, semicolcheia, 82);
        tocarNota(fifth, semicolcheia, 90);
        tocarNota(seventh, colcheia, 112);

        // Descida e repetição rítmica.
        tocarNota(fifth, semicolcheia, 80);
        tocarNota(third, semicolcheia, 88);
        tocarNota(fifth, semicolcheia, 96);
        tocarNota(fifth, semicolcheia, 72);

        // Fecha cada frase no primeiro tom do arpejo.
        if (i + 1 == solo.size()) {
            // Resolução longa no último acorde.
            tocarNota(tonic, minima * 2, 118);
        } else {
            tocarNota(tonic, seminima, 102);
            currentTick += colcheia;
        }
    }

    writer.save(filename);
}