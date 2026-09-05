#include "graph.h"
#include <cmath>
#include <limits>

double transitionCost(const Arpeggio& a, const Arpeggio& b) {
    double fretDist = std::abs(a.regionFret - b.regionFret);
    return fretDist;
}

ArpeggioGraph buildArpeggioGraph(const std::vector<std::string>& progression) {
    ArpeggioGraph graph;
    (void)progression;
    graph.source = 0;
    graph.sink = 1;
    return graph;
}
