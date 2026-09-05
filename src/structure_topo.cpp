#include "structure_topo.h"
#include <set>
#include <algorithm>
#include <iostream>
#include <cassert>

CondensedGraph condenseSCCs(const SectionGraph& originalGraph, const std::vector<std::vector<int>>& sccs) {
    CondensedGraph condensed;
    condensed.sccs = sccs;
    int numSCCs = (int)sccs.size();
    int numOriginalNodes = (int)originalGraph.sections.size();

    condensed.nodeToScc.resize(numOriginalNodes, -1);
    for (int sccId = 0; sccId < numSCCs; ++sccId) {
        for (int node : sccs[sccId]) {
            condensed.nodeToScc[node] = sccId;
        }
    }

    condensed.adj.resize(numSCCs);
    std::vector<std::set<int>> sccAdjSet(numSCCs);

    for (int u = 0; u < numOriginalNodes; ++u) {
        int sccU = condensed.nodeToScc[u];
        for (int v : originalGraph.adj[u]) {
            int sccV = condensed.nodeToScc[v];
            if (sccU != sccV) {
                sccAdjSet[sccU].insert(sccV);
            }
        }
    }

    for (int sccId = 0; sccId < numSCCs; ++sccId) {
        for (int targetScc : sccAdjSet[sccId]) {
            condensed.adj[sccId].push_back(targetScc);
        }
    }

    return condensed;
}

static void dfsTopo(int u, const CondensedGraph& condensedGraph, std::vector<bool>& visited, std::vector<int>& order) {
    visited[u] = true;
    for (int v : condensedGraph.adj[u]) {
        if (!visited[v]) {
            dfsTopo(v, condensedGraph, visited, order);
        }
    }
    order.push_back(u);
}

std::vector<int> topologicalSort(const CondensedGraph& condensedGraph) {
    int numSCCs = (int)condensedGraph.sccs.size();
    std::vector<bool> visited(numSCCs, false);
    std::vector<int> order;

    for (int i = 0; i < numSCCs; ++i) {
        if (!visited[i]) {
            dfsTopo(i, condensedGraph, visited, order);
        }
    }

    std::reverse(order.begin(), order.end());
    return order;
}

std::vector<std::string> expandOrder(const CondensedGraph& condensedGraph, 
                                     const std::vector<int>& topoOrder, 
                                     const SectionGraph& originalGraph) {
    std::vector<std::string> fullSequence;
    
    for (int sccId : topoOrder) {
        for (int origNodeIndex : condensedGraph.sccs[sccId]) {
            fullSequence.push_back(originalGraph.sections[origNodeIndex]);
        }
    }

    return fullSequence;
}

bool verifyTopologicalOrder(const CondensedGraph& condensedGraph, const std::vector<int>& topoOrder) {
    int numSCCs = (int)condensedGraph.sccs.size();
    std::vector<int> pos(numSCCs, -1);
    for (size_t i = 0; i < topoOrder.size(); ++i) {
        pos[topoOrder[i]] = (int)i;
    }

    for (int u = 0; u < numSCCs; ++u) {
        for (int v : condensedGraph.adj[u]) {
            if (pos[u] >= pos[v]) {
                std::cerr << "ERRO NA ORDENACAO TOPOLOGICA: Superno " << u << " (pos " << pos[u] 
                          << ") aparece apos ou junto com seu sucessor " << v << " (pos " << pos[v] << ")" << std::endl;
                return false;
            }
        }
    }

    return true;
}
