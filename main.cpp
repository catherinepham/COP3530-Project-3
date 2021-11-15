#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <set>
#include <map>
using namespace std;

//make person object here
class Graph
{
private:
    map<int, vector<pair<int, int>>> graph;

public:
    void insertEdge(int from, int to, int weight);
    void printGraph();
};

    //insertEdge() adds a new edge between the from and to vertex.
    void Graph::insertEdge(int from, int to, int weight)
    {
        graph[from].push_back(make_pair(to,weight));
        if(graph.find(to) == graph.end())
        {
            graph[to] = {};
        }
    }

void Graph::printGraph()
{
    //TODO: printGraph() prints the graph in a format sorted by ascending vertex matrix of weights
    for(auto i: graph)
    {
        cout << i.first;
        for(int k = 0; k < i.second.size(); k++)
        {
            cout << " " << i.second[k].first;
        }
        cout << endl;
    }
}


int main()
{

    return 0;
}
