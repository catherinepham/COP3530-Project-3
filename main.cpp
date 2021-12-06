#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <set>
#include <map>
#include<fstream>
#include <stack>
#include<queue>
#include<algorithm>
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
    vector<string> DepthFirstSearch(Person from, string company);
    vector<string> BreadthFirstSearch(Person from, string company);
    bool insertConnection(Person from, Person to);
    void printConnections(Person from);
};


void Graph::printConnections(Person from)
{
    if(graph[from].size()== 0)
    {
        cout<< "This person doesn't exist in the network"<< endl;
        return;
    }

    for(int i=0; i<graph[from].size(); i++)
    {
        cout<< graph[from].at(i).name<< endl;
        if(graph[from].at(i).company!= "")
        {
            cout<< graph[from].at(i).company<<endl;
        }

        cout<< endl;
    }
    return;
}



//insertEdge() adds a new edge between the from and to vertex.
void Graph::insertEdge(Person from, Person to)
{
    //has to be a doubly directed graph because the connections all go 2 ways
    //check for connections inserted twice because of doubly directed
    graph[from].push_back(to);
    graph[to].push_back(from);
}

vector<string> Graph:: DepthFirstSearch(Person from, string company)
{
    vector<string> connectionsPath;
    set<Person> visited;
    stack<Person> s;

    visited.insert(from);
    s.push(from);

    while(!s.empty())
    {
        Person person1 = s.top();
        //cout << person1.name << " " << person1.company << endl;
        connectionsPath.push_back(person1.name);
        s.pop();

        if(person1.company == company)
        {
            return connectionsPath;
        }

        vector<Person> connections = graph[from];
        for(Person v: connections)
        {
            if(visited.count(v) == 0)
            {
                visited.insert(v);
                s.push(v);
            }
        }
    }
    return connectionsPath;
}

vector<string> Graph::BreadthFirstSearch(Person from, string company) {
    vector<string> connectionsPath;
    set<Person> visited;
    queue<Person> q;

    visited.insert(from);
    q.push(from);

    while(!q.empty()){
        Person person1 = q.front();
        //cout << person1.name << " " << person1.company << endl;
        connectionsPath.push_back(person1.name);
        q.pop();

        if(person1.company == company) {
            return connectionsPath;
        }

        vector<Person> connections = graph[from];
        for (Person v: connections) {
            //accessing a person in the connections' connections
            /*if(graph[v].size() > 0){
                vector<string> temp = BreadthFirstSearch(v, company);
                if (temp.size() > 0) {
                }
            }*/
            if(visited.count(v)==0) {
                visited.insert(v);
                q.push(v);
            }
        }
    }
    vector<string> empty;
    return empty;
}

bool Graph::insertConnection(Person from, Person to) {
    //to check that a connection already exists, find "to" in "graph[from]"
    bool connection = false;
    for (int i = 0; i < graph[from].size(); i++) {
        if (graph[from].at(i).name == to.name && graph[from].at(i).company == to.company) {
            connection = true;
            break;
        }
    }
    if (!connection) {
        insertEdge(from, to);
        //return true if connection was successfully made
        return true;
    }
    else {//return false if connection already exists
        return false;
    }
}

int main()
{
    int lines = 0;
    int menuNum = 0;
    cout << "Welcome to LinkedIn Network" << endl;
    cout << "Please enter the amount of commands" << endl;
    cin >> lines;
    cout << "Please select one of the options below" << endl;
    cout << "1. Load Person's Connection List" << endl << "2. Insert a connection" << endl;
    cout << "3. Search for a company in the network using Breadth First Search" << endl << "4. Search for a company in the network using Depth First Search" << endl;
    cin >> menuNum;

    Graph graph;
    vector<Person> people;
    string filePath= "Test1.csv";
    ifstream File(filePath);

    if(File.is_open())
    {
        string lineFromFile;
        getline(File, lineFromFile);

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
            getline(stream, connectionCompany);





            string connectionName= connectionFirst+ " " + connectionLast;
            connectionCompany= connectionCompany.erase(connectionCompany.size() - 1);
            if(connectionName[connectionName.size()-1]== ' ')
            {
                connectionName= connectionName.erase(connectionName.size()-1);
            }
            if(name[name.size()-1]== ' ')
            {
                name= name.erase(name.size()-1);
            }



            bool found1= false;
            bool found2= false;
            for(int i=0; i<people.size(); i++)
            {
                if(people.at(i).name==name && people.at(i).company== company)
                {
                    found1 = true;
                }
                if(people.at(i).name==connectionName && people.at(i).company== connectionCompany)
                {
                    found2 = true;
                }
            }

            //cout<< "name"<< name<< endl;
            Person person1= Person(name,company);
            //name.resize(13);
            //cout<< name.size()<< name<< endl;
            if(!found1) {
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
    
    for(int i = 0; i < lines; i++)
    {
        if(menuNum == 1)
        {
            string name;
            string company;
            string targetCompany;
            string empty;
            cout << "Who's network are you looking at?" << endl;
            getline(cin, empty);
            getline(cin, name);
            cout << "Where do they work?" << endl;
            getline(cin, company);
            Person from = Person(name, company);

            graph.printConnections(from);
        }
        else if(menuNum == 2)
        {
            string name1, company1, name2, company2,empty;
            cout << "What is the name of the first person you want to make a connection with?"<< endl;
            getline(cin, empty);
            getline(cin, name1);
            cout << "What company do they work for?" << endl;
            getline(cin, company1);
            cout << "What is the name of the second person you want to make a connection with?"<< endl;
            getline(cin, name2);
            cout << "What company do they work for?" << endl;
            getline(cin, company2);
            bool insert = graph.insertConnection(Person(name1, company1), Person(name2,company2));
            if (insert)
                cout << "Connection successfully made" << endl;
            else
                cout << "Connection already exists" << endl;
        }
        else if(menuNum == 3)
        {
            //BFS
            string name;
            string company;
            string targetCompany;
            string empty;
            cout << "Who's network are you looking at?" << endl;
            getline(cin, empty);
            getline(cin, name);
            cout << "Where do they work?" << endl;
            getline(cin, company);
            Person from = Person(name, company);
            cout << "What company are you looking for?" << endl;
            getline(cin, targetCompany);
            vector<string> connections = graph.BreadthFirstSearch(from, targetCompany);
            cout << "Your path of connections to " << targetCompany << ": " << endl;
            for (auto i: connections) {
                cout << i << endl;
            }
        }
        else if(menuNum == 4)
        {
            //DFS
            string name;
            string company;
            string targetCompany;
            string empty;
            cout << "Who's network are you looking at?" << endl;
            getline(cin, empty);
            getline(cin, name);
            cout << "Where do they work?" << endl;
            getline(cin, company);
            Person from = Person(name, company);
            cout << "What company are you looking for?" << endl;
            getline(cin, targetCompany);
            vector<string> connections = graph.DepthFirstSearch(from, targetCompany);
            cout << "Your path of connections to " << targetCompany << ": " << endl;
            for (auto i: connections)
            {
                cout << i << endl;
            }
        }
        else
        {
            cout << "Invalid input" << endl;
        }
        cin >> menuNum;
    }
    return 0;
}
