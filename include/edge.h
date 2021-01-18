#pragma once

/**
 * An Edge is a class that represents a directed, weighted edge in a graph.
 * It is templated on three types.
 * @tparam V a type to represent the vertex labels
 * @tparam E a type to represent the edge labels
 * @tparam W a type to represent the weight (usually an int, float, or double)
 */
template <typename V, typename E, typename W>
class Edge {
public:
    Edge(V source, E label, W weight, V target);

    V source;
    E label;
    W weight;
    V target;
};

template <typename V, typename E, typename W>
Edge<V,E,W>::Edge(V source, E label, W weight, V target) {
    this->source = source;
    this->label = label;
    this->weight = weight;
    this->target = target;
}

