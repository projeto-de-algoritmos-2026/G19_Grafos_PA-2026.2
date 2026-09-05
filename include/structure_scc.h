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

SectionGraph buildStructureGraph(const std::unordered_map<std::string, std::vector<std::string>>& transitions);

std::vector<std::vector<int>> findSCCs(const SectionGraph& graph);

void printSCCs(const SectionGraph& graph, const std::vector<std::vector<int>>& sccs);

#endif
