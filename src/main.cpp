#include <iostream>
#include <string>
#include <vector>

#include "dijkstra_solo.h"
#include "fretboard.h"
#include "graph.h"
#include "midi_export.h"
#include "prim_variations.h"

int main(int argc, char* argv[]) {
  std::vector<std::string> progression;
  if (argc > 1) {
    progression.assign(argv + 1, argv + argc);
  } else {
    progression = {"Am7", "D7", "Gmaj7", "Cmaj7"};
  }

  ArpeggioGraph arpGraph = buildArpeggioGraph(progression);
  std::vector<Arpeggio> baseSolo = solveSoloDijkstra(arpGraph);
  if (baseSolo.empty()) {
    std::cerr << "Nao foi possivel gerar o solo para a progressao informada."
              << std::endl;
    return 1;
  }

  VariationGraph varGraph = buildVariationGraph(baseSolo, 10.0);
  std::vector<MSTEdge> mstEdges = primMST(varGraph, 0);
  std::vector<std::vector<Arpeggio>> sampled =
      sampleVariations(varGraph, mstEdges, 0, 3);

  exportMidi(baseSolo, "build/dijkstra_ori.mid", 120);
  for (size_t i = 0; i < sampled.size(); ++i) {
    exportMidi(sampled[i],
               "build/prim_variation" + std::to_string(i + 1) + ".mid",
               120);
  }

  std::cout << "RESULTADO DO SOLO" << std::endl;
  std::cout << "\nProgressao: ";
  for (size_t i = 0; i < progression.size(); ++i) {
    if (i > 0) {
      std::cout << " -> ";
    }
    std::cout << progression[i];
  }
  std::cout << "\n\nSolo-base escolhido pelo Dijkstra (" << baseSolo.size()
            << " arpejos)" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  for (size_t i = 0; i < baseSolo.size(); ++i) {
    std::cout << "  " << i + 1 << ". Acorde: " << baseSolo[i].chordName
              << " | regiao: casa " << baseSolo[i].regionFret << std::endl;
    std::cout << "     Notas: ";
    for (size_t j = 0; j < baseSolo[i].notes.size(); ++j) {
      const NotePos& note = baseSolo[i].notes[j];
      if (j > 0) {
        std::cout << ", ";
      }
      std::cout << midiToNote(note.midi) << " [corda " << note.string
                << ", casa " << note.fret << "]";
    }
    std::cout << std::endl;
  }

  std::cout << "\nVariacoes selecionadas pelo Prim (" << sampled.size()
            << ")" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  for (size_t i = 0; i < sampled.size(); ++i) {
    size_t noteCount = 0;
    for (const Arpeggio& arpeggio : sampled[i]) {
      noteCount += arpeggio.notes.size();
    }
    std::cout << "  " << i + 1 << ". " << sampled[i].size()
              << " arpejos | " << noteCount << " notas" << std::endl;
    std::cout << "     Notas: ";
    for (size_t j = 0; j < sampled[i].size(); ++j) {
      for (size_t k = 0; k < sampled[i][j].notes.size(); ++k) {
        if (j > 0 || k > 0) {
          std::cout << ", ";
        }
        std::cout << midiToNote(sampled[i][j].notes[k].midi);
      }
    }
    std::cout << std::endl;
    std::cout << "     Arquivo: build/prim_variation" << i + 1 << ".mid"
              << std::endl;
  }

  std::cout << "\nArquivo do solo-base: build/dijkstra_ori.mid" << std::endl;

  return 0;
}
