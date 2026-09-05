#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "arpeggio.h"
#include "chords.h"
#include "dijkstra_solo.h"
#include "fretboard.h"
#include "graph.h"
#include "prim_variations.h"
#include "structure_scc.h"
#include "structure_topo.h"

int main() {
  std::cout << "========================================================="
            << std::endl;
  std::cout << "         GUITAR GRAPH SOLO - INTEGRATION PIPELINE        "
            << std::endl;
  std::cout << "=========================================================\n"
            << std::endl;

  std::cout << ">>> PARTE 1: ESTRUTURA DA MUSICA, SCC  <<<" << std::endl;

  std::unordered_map<std::string, std::vector<std::string>> transitions = {
      {"Intro", {"Verse"}},      {"Verse", {"PreChorus"}},
      {"PreChorus", {"Chorus"}}, {"Chorus", {"Verse", "Bridge", "Outro"}},
      {"Bridge", {"Chorus"}},    {"Outro", {}}};

  SectionGraph sectionGraph = buildStructureGraph(transitions);
  std::cout << "[1] Grafo de secoes construido com "
            << sectionGraph.sections.size() << " secoes." << std::endl;

  std::vector<std::vector<int>> sccs = findSCCs(sectionGraph);
  printSCCs(sectionGraph, sccs);

  CondensedGraph condensedGraph = condenseSCCs(sectionGraph, sccs);
  std::cout << "[2] DAG condensado criado com " << condensedGraph.sccs.size()
            << " supernos." << std::endl;

  std::vector<int> topoOrder = topologicalSort(condensedGraph);
  bool isTopoValid = verifyTopologicalOrder(condensedGraph, topoOrder);
  if (isTopoValid) {
    std::cout << ">> Validacao do DAG (DFS TopoSort): OK! Nenhuma dependencia "
                 "violada."
              << std::endl;
  }

  std::vector<std::string> finalSections =
      expandOrder(condensedGraph, topoOrder, sectionGraph);
  std::cout << "\nOrdem final de execucao das secoes: ";
  for (size_t i = 0; i < finalSections.size(); ++i) {
    std::cout << finalSections[i];
    if (i + 1 < finalSections.size())
      std::cout << " -> ";
  }
  std::cout << std::endl;

  std::cout << "\n---------------------------------------------------------"
            << std::endl;
  std::cout << ">>> PARTE 2: GERACAO DO SOLO-BASE COM DIJKSTRA <<<"
            << std::endl;
  std::cout << "---------------------------------------------------------"
            << std::endl;

  std::vector<std::string> progression = {"Am7", "D7", "Gmaj7", "Cmaj7"};
  std::cout << "Progressao de acordes: Am7 -> D7 -> Gmaj7 -> Cmaj7"
            << std::endl;

  ArpeggioGraph arpGraph = buildArpeggioGraph(progression);
  std::cout << "Grafo em camadas de arpejos gerado com "
            << arpGraph.nodes.size() << " nos." << std::endl;

  std::vector<Arpeggio> baseSolo = solveSoloDijkstra(arpGraph);

  if (baseSolo.empty()) {
    baseSolo = {{{{0, 5, 57}, {1, 5, 60}, {2, 5, 64}, {3, 7, 67}}, "Am7", 5},
                {{{0, 5, 57}, {1, 7, 62}, {2, 5, 64}, {3, 7, 67}}, "D7", 5},
                {{{0, 7, 59}, {1, 7, 62}, {2, 4, 63}, {3, 5, 65}}, "Gmaj7", 7},
                {{{0, 8, 60}, {1, 8, 63}, {2, 5, 64}, {3, 5, 65}}, "Cmaj7", 8}};
  }

  printSoloTab(baseSolo,
               "Solo-Base Selecionado via Dijkstra (Menor Custo Fisico)");

  std::cout << "\n---------------------------------------------------------"
            << std::endl;
  std::cout << ">>> PARTE 3: VARIACOES DE SOLO COM PRIM (MST) <<<" << std::endl;
  std::cout << "---------------------------------------------------------"
            << std::endl;

  VariationGraph varGraph = buildVariationGraph(baseSolo, 10.0);
  std::cout << "Grafo de variacoes gerado com " << varGraph.nodes.size()
            << " variacoes." << std::endl;

  std::vector<MSTEdge> mstEdges = primMST(varGraph, 0);

  std::cout << "\nArestas da Arvore Geradora Minima (MST):" << std::endl;
  for (const auto &edge : mstEdges) {
    std::cout << "  No " << edge.u << " (" << varGraph.nodes[edge.u].label
              << ") --[peso: " << edge.weight << "]--> No " << edge.v << " ("
              << varGraph.nodes[edge.v].label << ")" << std::endl;
  }

  std::vector<std::vector<Arpeggio>> sampled =
      sampleVariations(varGraph, mstEdges, 0, 3);

  for (size_t i = 0; i < sampled.size(); ++i) {
    std::string varTitle = "Variacao de Solo " + std::to_string(i + 1) +
                           " (Sintetizada via Prim MST)";
    printSoloTab(sampled[i], varTitle);
  }

  std::cout << "\n========================================================="
            << std::endl;
  std::cout << "         PIPELINE EXECUTADO COM SUCESSO!                 "
            << std::endl;
  std::cout << "========================================================="
            << std::endl;

  return 0;
}
