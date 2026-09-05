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

CondensedGraph condenseSCCs(const SectionGraph& originalGraph, const std::vector<std::vector<int>>& sccs);

std::vector<int> topologicalSort(const CondensedGraph& condensedGraph);

std::vector<std::string> expandOrder(const CondensedGraph& condensedGraph, 
                                     const std::vector<int>& topoOrder, 
                                     const SectionGraph& originalGraph);

bool verifyTopologicalOrder(const CondensedGraph& condensedGraph, const std::vector<int>& topoOrder);

#endif
