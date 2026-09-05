#ifndef STRUCTURE_TOPO_H
#define STRUCTURE_TOPO_H

#include "structure_scc.h"
#include <vector>
#include <string>
#include <iostream>

struct CondensedGraph {
    std::vector<std::vector<int>> sccs;
    std::vector<std::vector<int>> adj;
    std::vector<int> nodeToScc;
};

// Condenses original graph into a Directed Acyclic Graph (DAG) of SCC supernodes
CondensedGraph condenseSCCs(const SectionGraph& originalGraph, const std::vector<std::vector<int>>& sccs);

// Performs Topological Sort on the condensed DAG using Kahn's Algorithm
std::vector<int> topologicalSort(const CondensedGraph& condensedGraph);

// Expands condensed supernode order into full sequence of original section names
std::vector<std::string> expandOrder(const CondensedGraph& condensedGraph, 
                                     const std::vector<int>& topoOrder, 
                                     const SectionGraph& originalGraph);

// Verification helper: verifies that no node appears after a successor in the DAG
bool verifyTopologicalOrder(const CondensedGraph& condensedGraph, const std::vector<int>& topoOrder);

#endif // STRUCTURE_TOPO_H
