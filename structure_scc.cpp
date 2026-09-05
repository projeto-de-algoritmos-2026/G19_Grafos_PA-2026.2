#include "structure_scc.h"
#include <stack>
#include <algorithm>

SectionGraph buildStructureGraph(const std::unordered_map<std::string, std::vector<std::string>>& transitions) {
    SectionGraph graph;
    
    // First pass: collect all unique section names
    for (const auto& [from, toList] : transitions) {
        if (graph.nameToIndex.find(from) == graph.nameToIndex.end()) {
            graph.nameToIndex[from] = (int)graph.sections.size();
            graph.sections.push_back(from);
        }
        for (const auto& to : toList) {
            if (graph.nameToIndex.find(to) == graph.nameToIndex.end()) {
                graph.nameToIndex[to] = (int)graph.sections.size();
                graph.sections.push_back(to);
            }
        }
    }

    int n = (int)graph.sections.size();
    graph.adj.resize(n);

    // Second pass: populate adjacency list
    for (const auto& [from, toList] : transitions) {
        int u = graph.nameToIndex[from];
        for (const auto& to : toList) {
            int v = graph.nameToIndex[to];
            graph.adj[u].push_back(v);
        }
    }

    return graph;
}

// First DFS for Kosaraju's algorithm to compute finish times
static void dfsKosaraju1(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::vector<int>& order) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfsKosaraju1(v, adj, visited, order);
        }
    }
    order.push_back(u);
}

// Second DFS for Kosaraju's algorithm on transposed graph
static void dfsKosaraju2(int u, const std::vector<std::vector<int>>& adjRev, std::vector<bool>& visited, std::vector<int>& currentSCC) {
    visited[u] = true;
    currentSCC.push_back(u);
    for (int v : adjRev[u]) {
        if (!visited[v]) {
            dfsKosaraju2(v, adjRev, visited, currentSCC);
        }
    }
}

std::vector<std::vector<int>> findSCCs(const SectionGraph& graph) {
    int n = (int)graph.sections.size();
    std::vector<std::vector<int>> sccs;
    if (n == 0) return sccs;

    // Step 1: Compute finish order using DFS
    std::vector<bool> visited(n, false);
    std::vector<int> order;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfsKosaraju1(i, graph.adj, visited, order);
        }
    }

    // Step 2: Build reversed (transposed) graph
    std::vector<std::vector<int>> adjRev(n);
    for (int u = 0; u < n; ++u) {
        for (int v : graph.adj[u]) {
            adjRev[v].push_back(u);
        }
    }

    // Step 3: DFS on transposed graph in reverse order of finish times
    std::fill(visited.begin(), visited.end(), false);
    std::reverse(order.begin(), order.end());

    for (int u : order) {
        if (!visited[u]) {
            std::vector<int> currentSCC;
            dfsKosaraju2(u, adjRev, visited, currentSCC);
            sccs.push_back(currentSCC);
        }
    }

    return sccs;
}

void printSCCs(const SectionGraph& graph, const std::vector<std::vector<int>>& sccs) {
    std::cout << "\n=== Componentes Fortemente Conectados (SCCs Encontrados) ===" << std::endl;
    for (size_t i = 0; i < sccs.size(); ++i) {
        std::cout << "SCC " << i + 1 << ": [ ";
        for (size_t j = 0; j < sccs[i].size(); ++j) {
            std::cout << graph.sections[sccs[i][j]];
            if (j + 1 < sccs[i].size()) std::cout << ", ";
        }
        std::cout << " ]" << (sccs[i].size() > 1 ? " (Ciclo Detectado)" : "") << std::endl;
    }
}
