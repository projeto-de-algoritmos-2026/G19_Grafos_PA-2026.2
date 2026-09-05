#ifndef STRUCTURE_SCC_H
#define STRUCTURE_SCC_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

struct SectionGraph {
    std::vector<std::string> sections;
    std::unordered_map<std::string, int> nameToIndex;
    std::vector<std::vector<int>> adj;
};

// Constructs the directed graph of song sections from transitions map
SectionGraph buildStructureGraph(const std::unordered_map<std::string, std::vector<std::string>>& transitions);

// Finds Strongly Connected Components using Kosaraju or Tarjan algorithm
std::vector<std::vector<int>> findSCCs(const SectionGraph& graph);

// Helper function to print discovered SCCs
void printSCCs(const SectionGraph& graph, const std::vector<std::vector<int>>& sccs);

#endif // STRUCTURE_SCC_H
