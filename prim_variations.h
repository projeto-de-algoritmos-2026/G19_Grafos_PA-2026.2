#ifndef PRIM_VARIATIONS_H
#define PRIM_VARIATIONS_H

#include <string>
#include <vector>
#include <utility>
#include <iostream>

struct NotePos {
    int string; // 0 to 5 (E grave ate E aguda)
    int fret;   // casa: 0 a 12/24
    int midi;   // valor MIDI da nota
};

struct Arpeggio {
    std::vector<NotePos> notes;
    std::string chordName;
    int regionFret;
};

struct VariationGraph {
    struct Node {
        int id;
        Arpeggio arpeggio;
        int sectionIndex;
        std::string label;
    };

    struct Edge {
        int to;
        double weight;
    };

    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adj;
};

// Functions for solo variations using Prim
VariationGraph buildVariationGraph(const std::vector<Arpeggio>& baseSolo, double radius = 2.0);

// Returns list of edges (from, to, weight) in the Minimum Spanning Tree
struct MSTEdge {
    int u;
    int v;
    double weight;
};

std::vector<MSTEdge> primMST(const VariationGraph& graph, int startNode = 0);

// Samples 2-3 distinct variation paths/solos from the MST
std::vector<std::vector<Arpeggio>> sampleVariations(const VariationGraph& graph, const std::vector<MSTEdge>& mstEdges, int startNode = 0, int numPaths = 3);

// Helper function to print an arpeggio solo
void printSoloTab(const std::vector<Arpeggio>& solo, const std::string& soloName);

#endif // PRIM_VARIATIONS_H
