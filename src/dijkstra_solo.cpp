#include "dijkstra_solo.h"
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

std::vector<Arpeggio> solveSoloDijkstra(const ArpeggioGraph& graph) {
    int n = (int)graph.nodes.size();
    if (n == 0) return {};

    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> parent(n, -1);

    using Element = std::pair<double, int>;
    std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

    dist[graph.source] = 0.0;
    pq.push({0.0, graph.source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == graph.sink) break;

        for (const auto& edge : graph.adj[u]) {
            int v = edge.to;
            double w = edge.weight;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    std::vector<Arpeggio> solo;
    int curr = parent[graph.sink];

    std::vector<int> pathNodes;
    while (curr != -1 && curr != graph.source) {
        pathNodes.push_back(curr);
        curr = parent[curr];
    }

    std::reverse(pathNodes.begin(), pathNodes.end());

    for (int nodeId : pathNodes) {
        solo.push_back(graph.nodes[nodeId].arpeggio);
    }

    return solo;
}
