#include "structure_scc.h"
#include <stack>
#include <algorithm>

SectionGraph buildStructureGraph(const std::unordered_map<std::string, std::vector<std::string>>& transitions) {
    SectionGraph graph;
    
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

    for (const auto& [from, toList] : transitions) {
        int u = graph.nameToIndex[from];
        for (const auto& to : toList) {
            int v = graph.nameToIndex[to];
            graph.adj[u].push_back(v);
        }
    }

    return graph;
}

static void tarjanDFS(int u, const std::vector<std::vector<int>>& adj,
                      int& timer, std::vector<int>& disc, std::vector<int>& low,
                      std::stack<int>& st, std::vector<bool>& inStack,
                      std::vector<std::vector<int>>& sccs) {
    disc[u] = low[u] = ++timer;
    st.push(u);
    inStack[u] = true;

    for (int v : adj[u]) {
        if (disc[v] == -1) {
            tarjanDFS(v, adj, timer, disc, low, st, inStack, sccs);
            low[u] = std::min(low[u], low[v]);
        } else if (inStack[v]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }

    if (low[u] == disc[u]) {
        std::vector<int> component;
        while (true) {
            int v = st.top();
            st.pop();
            inStack[v] = false;
            component.push_back(v);
            if (u == v) break;
        }
        sccs.push_back(component);
    }
}

std::vector<std::vector<int>> findSCCs(const SectionGraph& graph) {
    int n = (int)graph.sections.size();
    std::vector<std::vector<int>> sccs;
    if (n == 0) return sccs;

    int timer = 0;
    std::vector<int> disc(n, -1);
    std::vector<int> low(n, -1);
    std::vector<bool> inStack(n, false);
    std::stack<int> st;

    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) {
            tarjanDFS(i, graph.adj, timer, disc, low, st, inStack, sccs);
        }
    }

    return sccs;
}

void printSCCs(const SectionGraph& graph, const std::vector<std::vector<int>>& sccs) {
    std::cout << "\n=== Componentes Fortemente Conectados (SCCs) ===" << std::endl;
    for (size_t i = 0; i < sccs.size(); ++i) {
        std::cout << "SCC " << i + 1 << ": [ ";
        for (size_t j = 0; j < sccs[i].size(); ++j) {
            std::cout << graph.sections[sccs[i][j]];
            if (j + 1 < sccs[i].size()) std::cout << ", ";
        }
        std::cout << " ]" << std::endl;
    }
}
