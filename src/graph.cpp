#include "graph.h"
#include <cmath>
#include <limits>
#include <algorithm>

double transitionCost(const Arpeggio& a, const Arpeggio& b) {
    double fretDist = std::abs(a.regionFret - b.regionFret);
    double jumpPenalty = fretDist > 4 ? 5.0 : 0.0;

    double noteDist = 0.0;
    size_t minLen = std::min(a.notes.size(), b.notes.size());
    for (size_t i = 0; i < minLen; ++i) {
        noteDist += std::abs(a.notes[i].fret - b.notes[i].fret);
        noteDist += std::abs(a.notes[i].string - b.notes[i].string) * 1.5;
    }

    return fretDist + noteDist + jumpPenalty;
}

ArpeggioGraph buildArpeggioGraph(const std::vector<std::string>& progression) {
    ArpeggioGraph graph;
    if (progression.empty()) return graph;

    std::vector<std::vector<Arpeggio>> layers;
    for (const auto& chordName : progression) {
        layers.push_back(generateArpeggios(chordName));
    }

    int currentId = 0;

    ArpeggioGraph::Node sourceNode;
    sourceNode.id = currentId++;
    sourceNode.chordIndex = -1;
    sourceNode.isSource = true;
    sourceNode.isSink = false;
    graph.nodes.push_back(sourceNode);
    graph.source = sourceNode.id;

    std::vector<std::vector<int>> layerNodeIds(layers.size());

    for (size_t l = 0; l < layers.size(); ++l) {
        for (const auto& arp : layers[l]) {
            ArpeggioGraph::Node n;
            n.id = currentId++;
            n.arpeggio = arp;
            n.chordIndex = (int)l;
            n.isSource = false;
            n.isSink = false;
            graph.nodes.push_back(n);
            layerNodeIds[l].push_back(n.id);
        }
    }

    ArpeggioGraph::Node sinkNode;
    sinkNode.id = currentId++;
    sinkNode.chordIndex = (int)progression.size();
    sinkNode.isSource = false;
    sinkNode.isSink = true;
    graph.nodes.push_back(sinkNode);
    graph.sink = sinkNode.id;

    graph.adj.resize(currentId);

    for (int firstId : layerNodeIds[0]) {
        graph.adj[graph.source].push_back({graph.source, firstId, 0.0});
    }

    for (size_t l = 0; l + 1 < layers.size(); ++l) {
        for (size_t i = 0; i < layers[l].size(); ++i) {
            int uId = layerNodeIds[l][i];
            const Arpeggio& arpU = layers[l][i];

            for (size_t j = 0; j < layers[l + 1].size(); ++j) {
                int vId = layerNodeIds[l + 1][j];
                const Arpeggio& arpV = layers[l + 1][j];

                double w = transitionCost(arpU, arpV);
                graph.adj[uId].push_back({uId, vId, w});
            }
        }
    }

    int lastLayerIdx = (int)layers.size() - 1;
    for (int lastId : layerNodeIds[lastLayerIdx]) {
        graph.adj[lastId].push_back({lastId, graph.sink, 0.0});
    }

    return graph;
}
