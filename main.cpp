#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <set>
#include <map>
using namespace std;

struct Person {
    string name;
    string company;

    //constructor
    Person(string _name, string _company){
        name = _name;
        company = _company;
    }
};

class Graph
{
private:
    map<Person, vector<Person>> graph;

public:
    void insertEdge(string from, string to);
    void printGraph();
};

//insertEdge() adds a new edge between the from and to vertex.
void Graph::insertEdge(string from, string to) {

}

void Graph::printGraph()
{
    //TODO: printGraph() prints the graph in a format sorted by ascending vertex matrix of weights
    /*for(auto i: graph)
    {
        cout << i.first;
        for(int k = 0; k < i.second.size(); k++)
        {
            cout << " " << i.second[k].first;
        }
        cout << endl;
    }*/
}

int main()
{

    return 0;
}
