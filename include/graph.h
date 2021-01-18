#pragma once

#include <vector>
#include <iostream>
#include "edge.h"

using std::vector;
using std::ostream;

/**
 * Graph is a class representing directed, weighted graphs.
 * @tparam V a type to represent the vertex labels
 * @tparam E a type to represent the edge labels
 * @tparam W a type to represent the weight (usually an int, float, or double)
 */
template <typename V, typename E, typename W>
class Graph {
public:
    virtual ~Graph() {}

    /**
     * Retrieves all vertices in the graph.
     * @return The vertices in this graph.
     */
    virtual vector<V> getVertices() = 0;

    /**
     * Adds a vertex to this graph.
     * @param v The vertex to add.
     */
    virtual void insertVertex(V v) = 0;

    /**
     * Removes a vertex from the graph.
     * @param v The vertex to remove.
     * @throws runtime_error If the provided vertex is not in the graph.
     */
    virtual void removeVertex(V v) = 0;

    /**
     * Determines if a vertex is in this graph.
     * @param v The vertex to check.
     * @return true if the vertex is in the graph; false if it is not.
     */
    virtual bool containsVertex(V v) = 0;

    /**
     * Adds a directed edge to this graph.
     * @param source The source vertex for the edge.
     * @param destination The destination vertex for the edge.
     * @param label The label of the edge to add.
     * @param weight The weight of the edge to add.
     * @throws runtime_error If an edge already exists between the given source
     *                       and target.
     */
    virtual void insertEdge(V src, V dest, E label, W weight) = 0;

    /**
     * Removes an edge from this graph.
     * @param source The source vertex for the edge.
     * @param destination The destination vertex for the edge.
     * @throws runtime_error If the edge does not exist.
     */
    virtual void removeEdge(V src, V dest) = 0;

    /**
     * Determines if this graph contains an edge.
     * @param source The source vertex for the edge.
     * @param destination The destination vertex for the edge.
     * @return true if the graph contains that edge; false if it does not.
     */
    virtual bool containsEdge(V source, V destination) = 0;

    /**
     * Retrieves an edge from this graph.
     * @param source The source vertex for the edge.
     * @param destination The destination vertex for the edge.
     * @return The edge.
     * @throws runtime_error If the edge does not exist.
     */
    virtual Edge<V,E,W> getEdge(V source, V destination) = 0;

    /**
     * Retrieves all edges from this graph.
     * @return A vector containing every edge in this graph.
     */
    virtual vector<Edge<V,E,W> > getEdges() = 0;

    /**
     * Retrieves all edges with a common source.
     * @param source The source vertex.
     * @return All edges in the graph which leave that source vertex.
     */
    virtual vector<Edge<V,E,W> > getOutgoingEdges(V source) = 0;

    /**
     * Retrieves all edges with a common destination.
     * @param destination The destination vertex for the edge.
     * @return All edges in the graph which enter that destination vertex.
     */
    virtual vector<Edge<V,E,W> > getIncomingEdges(V destination) = 0;

    /**
     * Retrieves all vertices which are reachable by an edge with the provided
     * source vertex.
     * @param source The source vertex.
     * @return All vertices in the graph which are a destination for an edge
     *         that has the given source.
     */
    virtual vector<V> getNeighbors(V source) = 0;

    // You can safely ignore the following code.  This eliminates some default
    // class routines, preventing you from using them accidentally.
    // Specifically, we are disabling the use of the copy constructor and the
    // copy assignment operator.  You can read more here:
    //   http://www.cplusplus.com/articles/y8hv0pDG/
public:
    Graph() { }
private:
    Graph(const Graph& other) = delete;
    Graph& operator=(const Graph& other) = delete;
};

