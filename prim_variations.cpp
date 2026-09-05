#include "prim_variations.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <iostream>

static double calculateArpeggioDistance(const Arpeggio& a1, const Arpeggio& a2) {
    double dist = std::abs(a1.regionFret - a2.regionFret) * 1.5;
    size_t minLen = std::min(a1.notes.size(), a2.notes.size());
    for (size_t i = 0; i < minLen; ++i) {
        dist += std::abs(a1.notes[i].midi - a2.notes[i].midi) * 0.5;
        dist += std::abs(a1.notes[i].fret - a2.notes[i].fret) * 0.8;
    }
    dist += std::abs((int)a1.notes.size() - (int)a2.notes.size()) * 2.0;
    return dist;
}

static double calculateSoloDistance(const std::vector<Arpeggio>& s1, const std::vector<Arpeggio>& s2) {
    double total = 0.0;
    size_t len = std::min(s1.size(), s2.size());
    for (size_t i = 0; i < len; ++i) {
        total += calculateArpeggioDistance(s1[i], s2[i]);
    }
    return total;
}

VariationGraph buildVariationGraph(const std::vector<Arpeggio>& baseSolo, double radius) {
    VariationGraph graph;

    // Node 0: Base solo
    graph.nodes.push_back({0, baseSolo.empty() ? Arpeggio{} : baseSolo[0], 0, "Solo Base Original"});
    
    // Helper to store full solos for each node
    std::vector<std::vector<Arpeggio>> nodeSolos;
    nodeSolos.push_back(baseSolo);

    // Variation 1: Inverted note order for each arpeggio
    std::vector<Arpeggio> varInverted = baseSolo;
    for (auto& arp : varInverted) {
        std::reverse(arp.notes.begin(), arp.notes.end());
    }
    int id1 = (int)graph.nodes.size();
    graph.nodes.push_back({id1, varInverted.empty() ? Arpeggio{} : varInverted[0], 0, "Variação 1: Ordem Invertida"});
    nodeSolos.push_back(varInverted);

    // Variation 2: Shift position by +2 frets (high region)
    std::vector<Arpeggio> varShifted = baseSolo;
    for (auto& arp : varShifted) {
        arp.regionFret += 2;
        for (auto& n : arp.notes) {
            n.fret += 2;
            n.midi += 2;
        }
    }
    int id2 = (int)graph.nodes.size();
    graph.nodes.push_back({id2, varShifted.empty() ? Arpeggio{} : varShifted[0], 0, "Variação 2: Deslocada (+2 casas)"});
    nodeSolos.push_back(varShifted);

    // Variation 3: Passing note insertion (nota de aproximação cromática)
    std::vector<Arpeggio> varPassing = baseSolo;
    for (auto& arp : varPassing) {
        if (!arp.notes.empty()) {
            NotePos passing = arp.notes[0];
            passing.fret += 1;
            passing.midi += 1;
            arp.notes.push_back(passing);
        }
    }
    int id3 = (int)graph.nodes.size();
    graph.nodes.push_back({id3, varPassing.empty() ? Arpeggio{} : varPassing[0], 0, "Variação 3: Nota de Aproximação Inserida"});
    nodeSolos.push_back(varPassing);

    // Variation 4: Lower region shift (-1 fret)
    std::vector<Arpeggio> varLower = baseSolo;
    for (auto& arp : varLower) {
        if (arp.regionFret > 0) arp.regionFret -= 1;
        for (auto& n : arp.notes) {
            if (n.fret > 0) n.fret -= 1;
            if (n.midi > 0) n.midi -= 1;
        }
    }
    int id4 = (int)graph.nodes.size();
    graph.nodes.push_back({id4, varLower.empty() ? Arpeggio{} : varLower[0], 0, "Variação 4: Região Grave (-1 casa)"});
    nodeSolos.push_back(varLower);

    int n = (int)graph.nodes.size();
    graph.adj.resize(n);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double w = calculateSoloDistance(nodeSolos[i], nodeSolos[j]);
            if (w <= radius * 10.0) { // Connect nodes within acceptable threshold
                graph.adj[i].push_back({j, w});
                graph.adj[j].push_back({i, w});
            }
        }
    }

    return graph;
}

std::vector<MSTEdge> primMST(const VariationGraph& graph, int startNode) {
    std::vector<MSTEdge> mst;
    int n = (int)graph.nodes.size();
    if (n == 0) return mst;

    std::vector<bool> inMST(n, false);
    std::vector<double> minEdge(n, 1e9);
    std::vector<int> parent(n, -1);

    // Priority queue storing pair<weight, node>
    using Element = std::pair<double, int>;
    std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

    minEdge[startNode] = 0.0;
    pq.push({0.0, startNode});

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;

        if (parent[u] != -1) {
            mst.push_back({parent[u], u, w});
        }

        for (const auto& edge : graph.adj[u]) {
            int v = edge.to;
            double weight = edge.weight;
            if (!inMST[v] && weight < minEdge[v]) {
                minEdge[v] = weight;
                parent[v] = u;
                pq.push({weight, v});
            }
        }
    }

    return mst;
}

std::vector<std::vector<Arpeggio>> sampleVariations(const VariationGraph& graph, const std::vector<MSTEdge>& mstEdges, int startNode, int numPaths) {
    (void)startNode;
    std::vector<std::vector<Arpeggio>> variations;
    int count = 0;
    for (const auto& edge : mstEdges) {
        if (count >= numPaths) break;
        int targetNode = edge.v;
        if (targetNode >= 0 && targetNode < (int)graph.nodes.size()) {
            std::vector<Arpeggio> soloSample;
            soloSample.push_back(graph.nodes[targetNode].arpeggio);
            variations.push_back(soloSample);
            count++;
        }
    }
    return variations;
}

void printSoloTab(const std::vector<Arpeggio>& solo, const std::string& soloName) {
    std::cout << "\n=== " << soloName << " ===" << std::endl;
    for (size_t i = 0; i < solo.size(); ++i) {
        std::cout << "Acorde: " << solo[i].chordName << " (Região da casa " << solo[i].regionFret << ")" << std::endl;
        std::cout << "  Notas (String, Casa, MIDI): ";
        for (const auto& note : solo[i].notes) {
            std::cout << "[S:" << note.string << " C:" << note.fret << " M:" << note.midi << "] ";
        }
        std::cout << std::endl;
    }
}
