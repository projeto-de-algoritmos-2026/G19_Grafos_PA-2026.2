#ifndef GRAPH_H
#define GRAPH_H

#include "arpeggio.h"
#include <vector>
#include <string>

struct ArpeggioGraph {
    struct Node {
        int id;
        Arpeggio arpeggio;
        int chordIndex;
        bool isSource;
        bool isSink;
    };

    struct Edge {
        int from;
        int to;
        double weight;
    };

    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adj;
    int source;
    int sink;
};

ArpeggioGraph buildArpeggioGraph(const std::vector<std::string>& progression);
double transitionCost(const Arpeggio& a, const Arpeggio& b);

#endif
