#include "GraphGridLayout.h"
#include <vector>
#include <map>

#include <emscripten/bind.h>

using namespace emscripten;

typedef GraphGridLayout::GraphBlock GraphBlock;
typedef GraphGridLayout::GraphEdge GraphEdge;

struct Point
{
    qreal x, y;

    Point() : x(0), y(0)
    {
    }

    Point(qreal x, qreal y) : x(x), y(y)
    {
    }

    qreal getX() const
    {
        return x;
    }

    qreal getY() const
    {
        return y;
    }
};

EMSCRIPTEN_BINDINGS(point) {
  class_<Point>("Point")
    .property("x", &Point::getX)
    .property("y", &Point::getY)
    ;
}

struct Node
{
    using NodeId = int;

    int x, y;
    int width, height;
    std::map<NodeId, std::vector<Point>> edges;

    Node(int x, int y, int width, int height, std::map<NodeId, std::vector<Point>> edges)
        : x(x), y(y), width(width), height(height), edges(std::move(edges))
    {
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    std::map<NodeId, std::vector<Point>> getEdges() const
    {
        return edges;
    }
};

EMSCRIPTEN_BINDINGS(node) {
  class_<Node>("Node")
    .property("x", &Node::getX)
    .property("y", &Node::getY)
    .property("width", &Node::getWidth)
    .property("height", &Node::getHeight)
    .property("edges", &Node::getEdges)
    ;
}

class FlowGraph
{
private:
    int node_counter = 0;
    GraphGridLayout::Graph g;

public:
    int width, height;

    FlowGraph() : g()
    {
    }

    Node::NodeId addNode(int width, int height)
    {
        GraphBlock node;
        node.entry = node_counter++;
        node.width = width;
        node.height = height;
        g.insert({node.entry, node});
        return node.entry;
    }

    void addEdge(Node::NodeId source, Node::NodeId target)
    {
        g[source].edges.emplace_back(target);
    }

    std::vector<Node> calculateLayout(Node::NodeId entry)
    {
        GraphGridLayout layout;
        layout.CalculateLayout(g, 0, width, height);

        std::vector<Node> nodes;
        for (auto& [entry, block] : g)
        {
            std::map<Node::NodeId, std::vector<Point>> edges;
            for (auto& edge : block.edges)
            {
                std::vector<Point> points;
                for (auto& point : edge.polyline)
                {
                    points.emplace_back(point.x(), point.y());
                }
                edges.insert({edge.target, std::move(points)});
            }
            nodes.emplace_back(block.x, block.y, block.width, block.height, std::move(edges));
        }

        return nodes;
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }
};

EMSCRIPTEN_BINDINGS(flow_graph) {
  class_<FlowGraph>("FlowGraph")
    .property("width", &FlowGraph::getWidth)
    .property("height", &FlowGraph::getHeight)
    .function("addNode", &FlowGraph::addNode)
    .function("addEdge", &FlowGraph::addEdge)
    .function("calculateLayout", &FlowGraph::calculateLayout)
    ;
}

EMSCRIPTEN_BINDINGS(module) {
  register_vector<Point>("vector<Point>");
  register_map<Node::NodeId, Point>("map<NodeId, vector<Point>>");
}