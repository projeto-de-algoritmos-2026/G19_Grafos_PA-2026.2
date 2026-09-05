#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "prim_variations.h"
#include "structure_scc.h"
#include "structure_topo.h"

int main() {
  std::cout << "========================================================="
            << std::endl;
  std::cout << " GUITAR GRAPH SOLO - Estrutura Musical & Variações"
            << std::endl;
  std::cout << "=========================================================\n"
            << std::endl;

  std::cout << "[1] Construindo Grafo de Transições de Seções Musicais..."
            << std::endl;

  std::unordered_map<std::string, std::vector<std::string>> transitions = {
      {"Intro", {"Verse"}},      {"Verse", {"PreChorus"}},
      {"PreChorus", {"Chorus"}}, {"Chorus", {"Verse", "Bridge", "Outro"}},
      {"Bridge", {"Chorus"}},    {"Outro", {}}};

  SectionGraph sectionGraph = buildStructureGraph(transitions);
  std::cout << "Grafo construído com " << sectionGraph.sections.size()
            << " seções." << std::endl;

  std::cout << "\n[2] Executando detecção de SCCs (Componentes Fortemente "
               "Conectados)..."
            << std::endl;
  std::vector<std::vector<int>> sccs = findSCCs(sectionGraph);
  printSCCs(sectionGraph, sccs);

  std::cout << "\n[3] Condensando SCCs em um DAG (Directed Acyclic Graph)..."
            << std::endl;
  CondensedGraph condensedGraph = condenseSCCs(sectionGraph, sccs);
  std::cout << "DAG condensado possui " << condensedGraph.sccs.size()
            << " supernós." << std::endl;

  std::cout << "\n[4] Executando Ordenação Topológica nos supernós do DAG..."
            << std::endl;
  std::vector<int> topoOrder = topologicalSort(condensedGraph);

  bool isTopoValid = verifyTopologicalOrder(condensedGraph, topoOrder);
  if (isTopoValid) {
    std::cout << ">> VERIFICAÇÃO OK: Nenhum nó aparece após um sucessor no DAG!"
              << std::endl;
  } else {
    std::cerr << ">> ERRO: A ordenação topológica violou a ordem do DAG!"
              << std::endl;
  }

  std::cout << "\n[5] Expandindo ordem dos supernós em sequência de seções "
               "musicais..."
            << std::endl;
  std::vector<std::string> finalSections =
      expandOrder(condensedGraph, topoOrder, sectionGraph);

  std::cout << "Ordem final da apresentação das seções: ";
  for (size_t i = 0; i < finalSections.size(); ++i) {
    std::cout << finalSections[i];
    if (i + 1 < finalSections.size())
      std::cout << " -> ";
  }
  std::cout << std::endl;

  std::cout << "\n---------------------------------------------------------"
            << std::endl;
  std::cout << "[6] Gerando Variações de Solos com o Algoritmo de Prim"
            << std::endl;
  std::cout << "---------------------------------------------------------"
            << std::endl;

  std::vector<Arpeggio> baseSolo = {
      {{{0, 5, 57}, {1, 5, 60}, {2, 5, 64}, {3, 7, 67}}, "Am7", 5},
      {{{0, 5, 57}, {1, 7, 62}, {2, 5, 64}, {3, 7, 67}}, "D7", 5},
      {{{0, 7, 59}, {1, 7, 62}, {2, 4, 63}, {3, 5, 65}}, "Gmaj7", 7},
      {{{0, 8, 60}, {1, 8, 63}, {2, 5, 64}, {3, 5, 65}}, "Cmaj7", 8}};

  printSoloTab(baseSolo, "Solo Base Principal (Entrada para Variações)");

  VariationGraph varGraph = buildVariationGraph(baseSolo, 10.0);
  std::cout << "\nGrafo de Variações gerado com " << varGraph.nodes.size()
            << " nós de variações." << std::endl;

  std::cout << "\nExecutando Algoritmo de Prim para encontrar Árvore Geradora "
               "Mínima (MST)..."
            << std::endl;
  std::vector<MSTEdge> mstEdges = primMST(varGraph, 0);

  std::cout << "\nArestas da MST geradas pelo Prim:" << std::endl;
  for (const auto &edge : mstEdges) {
    std::cout << "  Nó " << edge.u << " (" << varGraph.nodes[edge.u].label
              << ") --[peso: " << edge.weight << "]--> Nó " << edge.v << " ("
              << varGraph.nodes[edge.v].label << ")" << std::endl;
  }

  std::cout
      << "\n[7] Extraindo 2-3 variações de solos completas a partir da MST..."
      << std::endl;
  std::vector<std::vector<Arpeggio>> sampled =
      sampleVariations(varGraph, mstEdges, 0, 3);

  for (size_t i = 0; i < sampled.size(); ++i) {
    std::string varName = "Variação Produzida " + std::to_string(i + 1);
    printSoloTab(sampled[i], varName);
  }

  std::cout << "\n========================================================="
            << std::endl;
  std::cout << " EXECUÇÃO CONCLUÍDA COM SUCESSO!" << std::endl;
  std::cout << "========================================================="
            << std::endl;

  return 0;
}
