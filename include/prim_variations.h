#ifndef PRIM_VARIATIONS_H
#define PRIM_VARIATIONS_H

#include "fretboard.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>

#ifndef NOTE_POS_DEFINED
#define NOTE_POS_DEFINED
struct NotePos {
    int string;
    int fret;
    int midi;
};
#endif

#ifndef ARPEGGIO_DEFINED
#define ARPEGGIO_DEFINED
struct Arpeggio {
    std::vector<NotePos> notes;
    std::string chordName;
    int regionFret;
};
#endif

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

VariationGraph buildVariationGraph(const std::vector<Arpeggio>& baseSolo, double radius = 2.0);

struct MSTEdge {
    int u;
    int v;
    double weight;
};

std::vector<MSTEdge> primMST(const VariationGraph& graph, int startNode = 0);

std::vector<std::vector<Arpeggio>> sampleVariations(const VariationGraph& graph, const std::vector<MSTEdge>& mstEdges, int startNode = 0, int numPaths = 3);

void printSoloTab(const std::vector<Arpeggio>& solo, const std::string& soloName);

#endif
