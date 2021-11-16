#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <set>
#include <map>
#include<fstream>
#include<queue>
using namespace std;


struct Person 
{
    string name;
    string company;

    //constructor
    Person(string _name, string _company)
    {
        name = _name;
        company = _company;
    }

    bool operator<(const Person &ob) const 
    {
        return name < ob.name || (name == ob.name && company < ob.company);
    }
};

class Graph
{
private:
    map<Person, vector<Person>> graph;

public:
    void insertEdge(Person from, Person to);
    void printGraph();
    vector<string> BreadthFirstSearch(Person from, string company);
    int getSize();
};


//insertEdge() adds a new edge between the from and to vertex.
void Graph::insertEdge(Person from, Person to) 
{
    //has to be a doubly directed graph because the connections all go 2 ways
    //check for connections inserted twice because of doubly directed
    graph[from].push_back(to);
    graph[to].push_back(from);
}

void Graph::printGraph()
{
    //algorithm for traversing
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

vector<string> Graph::BreadthFirstSearch(Person from, string company) {
    vector<string> connectionsPath;
    set<Person> visited;
    queue<Person> q;

    visited.insert(from);
    q.push(from);

    while(!q.empty()){
        Person person1 = q.front();
        cout << person1.name << " " << person1.company << endl;
        connectionsPath.push_back(person1.name);
        q.pop();

        vector<Person> connections = graph[from];
        for (Person v: connections) {
            if(visited.count(v)==0) {
                visited.insert(v);
                q.push(v);
            }
        }
    }
    return connectionsPath;
}

int Graph::getSize() {
    return graph.size();
}

int main() 
{
    int lines = 0;
        int menuNum = 0;
        cout << "Welcome to LinkedIn Network" << endl;
        cout << "Please enter the amount of commands" << endl;
        cin >> lines;
        cout << "Please select one of the options below" << endl;
        cout << "1. Load connections" << endl << "2. Insert a connection" << endl;
        cout << "3. Search for a company in the network" << endl;
        cin >> menuNum;

        for(int i = 0; i < lines; i++)
        {
            if(menuNum == 1)
            {

            }
            else if(menuNum == 2)
            {

            }
            else if(menuNum == 3)
            {

            }
            else
            {
                cout << "Invalid input" << endl;
            }

        }
    Graph graph;
    vector<Person> people;
    string filePath = "Camille Linkedin.csv";
    ifstream File(filePath);
    if(File.is_open())
    {
        string lineFromFile;
        while(getline(File, lineFromFile))
        {
            istringstream stream(lineFromFile); //stream of data coming from a string

            string name;
            string company;
            string connectionFirst;
            string connectionLast;
            string connectionCompany;

            getline(stream, name, ',');
            getline(stream, company, ',');
            getline(stream, connectionFirst, ',');
            getline(stream, connectionLast, ',');
            getline(stream, connectionCompany, ',');

            string connectionName = connectionFirst+ " " + connectionLast;

            bool found1= false;
            bool found2= false;
            for(int i=0; i<people.size(); i++)
            {
                if(people.at(i).name == name && people.at(i).company == company)
                {
                    found1 = true;
                }
                if(people.at(i).name == connectionName && people.at(i).company == connectionCompany)
                {
                    found2 = true;
                }
            }
            Person person1= Person(name,company);
            if(!found1)
            {
                people.push_back(person1);
            }
            Person connection= Person(connectionName, connectionCompany);
            if(!found2)
            {
                people.push_back(connection);
            }

            graph.insertEdge(person1, connection);
        }
    }


    return 0;
}
