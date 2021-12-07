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
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <cstdlib>
#include <ctime>
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
    Person()
    {
        name="";
        company="";
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
    void printConnections(Person from, sf:: RenderWindow &window);
};


void Graph::printConnections(Person from, sf:: RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("Font/arial.ttf");
    if(!font.loadFromFile("Font/arial.ttf"))
    {
        cout<< "ERROR IN FONT" << endl;
    }

    sf::Text text;
    text.setFont(font);

    if(graph[from].size()== 0)
    {
        string message= "This person doesn't exist in the network";
        text.setString(message);
        text.setCharacterSize(80);
        text.setFillColor(sf::Color::White);
        text.setPosition(200,200);
        window.draw(text);


        //cout<< "This person doesn't exist in the network"<< endl;
        return;
    }

    string message= "This person doesn't exist in the network";
    text.setString(message);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);


    int j=0;
    int m=0;

    for(int i=0; i<graph[from].size(); i++)
    {
        string message= graph[from].at(i).name+"-- "+graph[from].at(i).company;
        text.setString(message);
        text.setPosition(j,m*22);
        window.draw(text);
        if(i!=0 && i%70==0)
        {
            j+=700;
            m=0;
        }
        m++;
        //cout<< graph[from].at(i).name<< endl;
        /*if(graph[from].at(i).company!= "")
        {
            cout<< graph[from].at(i).company<<endl;
        }*/

        //cout<< endl;
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


    int lines = 7;
    int menuNum = 0;
    /*cout << "Welcome to LinkedIn Network" << endl;
    cout << "Please enter the amount of commands" << endl;
    cin >> lines;
    cout << "Please select one of the options below" << endl;
    cout << "1. Load Person's Connection List" << endl << "2. Insert a connection" << endl;
    cout << "3. Search for a company in the network using Breadth First Search" << endl << "4. Search for a company in the network using Depth First Search" << endl;
    cin >> menuNum;*/

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


    ///SFML SECION!!!

    sf::RenderWindow window(sf::VideoMode(2700, 1600), "Girl Boss, Gatekeep, Graph Functions: Linkedin Connection Analyzer");
    sf:: Sprite camille(TextureManager::GetTexture("camille4"));
    camille.setScale(1.43,1.43);
    camille.setPosition(400, 450);

    sf:: Sprite camilleName(TextureManager::GetTexture("camilleText"));
    camilleName.setScale(0.6,0.6);
    camilleName.setPosition(330, 140);

    sf:: Sprite cathy(TextureManager::GetTexture("cathy"));
    cathy.setPosition(1900, 450);

    sf:: Sprite nathalie(TextureManager::GetTexture("nathalie"));
    nathalie.setPosition(1150, 450);

    sf:: Sprite nathalieName(TextureManager::GetTexture("nathalieText"));
    nathalieName.setScale(0.6,0.6);
    nathalieName.setPosition(1080, 142);

    sf:: Sprite cathyName(TextureManager::GetTexture("cathyText"));
    cathyName.setScale(0.6,0.6);
    cathyName.setPosition(1830, 140);

    sf:: Sprite question(TextureManager::GetTexture("question"));
    //cathyName.setScale(0.6,0.6);
    question.setPosition(465, 10);

    sf:: Sprite menuButton(TextureManager::GetTexture("menu"));
    menuButton.setPosition(2520,1500);

    sf:: Sprite title(TextureManager::GetTexture("welcome_title"));
    title.setPosition(575, 100);
    title.setScale(2,2);
    sf:: Sprite directions(TextureManager::GetTexture("directions"));
    directions.setPosition(630, 400);
    directions.setScale(2,2);
    sf:: Sprite option1(TextureManager::GetTexture("option_1"));
    option1.setPosition(625, 550);
    option1.setScale(2,2);
    sf:: Sprite option2(TextureManager::GetTexture("option_2"));
    option2.setPosition(1430, 550);
    option2.setScale(2,2);
    sf:: Sprite option3(TextureManager::GetTexture("option_3"));
    option3.setPosition(625, 925);
    option3.setScale(2,2);
    sf:: Sprite option4(TextureManager::GetTexture("option_4"));
    option4.setPosition(1430, 925);
    option4.setScale(2,2);

    sf:: Sprite nameInput(TextureManager::GetTexture("nameInput"));
    nameInput.setPosition(200,570);
    sf:: Sprite companyInput(TextureManager::GetTexture("companyInput"));
    companyInput.setPosition(100,870);

    sf:: Sprite titleInput(TextureManager::GetTexture("titleInput"));
    titleInput.setPosition(320,50);

    sf:: Sprite failed(TextureManager::GetTexture("failed"));
    failed.setPosition(420,1200);

    sf:: Sprite success(TextureManager::GetTexture("success"));
    success.setPosition(530,1200);

    int screen=4;
    int menu=1;
    Person from;

    while(window.isOpen())
    {
        if(screen==1)
        {
            window.draw(title);
            window.draw(directions);
            window.draw(option1);
            window.draw(option2);
            window.draw(option3);
            window.draw(option4);
        }
        else if(screen==2)
        {
            window.draw(camille);
            window.draw(nathalie);
            window.draw(cathy);
            window.draw(camilleName);
            window.draw(nathalieName);
            window.draw(cathyName);
            window.draw(question);
        }
        else if(screen==3)
        {
            graph.printConnections(from, window);
            window.draw(menuButton);
        }

        else if(screen==4)
        {
            window.draw(nameInput);
            window.draw(companyInput);
            window.draw(titleInput);
            window.draw(failed);
            window.draw(menuButton);

        }

        sf:: Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type== sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    auto boundsCamille = camille.getGlobalBounds();
                    if(boundsCamille.contains(mousePos.x, mousePos.y))
                    {
                        from.name= "Camille Eyman";
                        from.company= "UF Center for Arts in Medicine";

                        if(menu==1)
                        {
                            screen=3;
                        }
                    }

                    auto boundsNathalie = nathalie.getGlobalBounds();
                    if(boundsNathalie.contains(mousePos.x, mousePos.y))
                    {
                        from.name= "Nathalie Leave";
                        from.company= "Microsoft";

                        if(menu==1)
                        {
                            screen=3;
                        }
                    }

                    auto boundsCathy = cathy.getGlobalBounds();
                    if(boundsCathy.contains(mousePos.x, mousePos.y))
                    {
                        from.name= "Catherine Pham";
                        from.company= "University of Florida";
                        if(menu==1)
                        {
                            screen=3;
                        }
                    }

                    auto boundsMenu = menuButton.getGlobalBounds();
                    if(boundsMenu.contains(mousePos.x, mousePos.y))
                    {
                        screen=1;
                    }

                    auto option1Bounds = option1.getGlobalBounds();
                    auto option2Bounds = option2.getGlobalBounds();
                    auto option3Bounds = option3.getGlobalBounds();
                    auto option4Bounds = option4.getGlobalBounds();

                    if (option1Bounds.contains(mousePos.x, mousePos.y)) {
                        menu=1;
                        screen=2;
                    }
                    else if (option2Bounds.contains(mousePos.x, mousePos.y)) {
                        menu=2;
                        screen=2;
                    }
                    else if (option3Bounds.contains(mousePos.x, mousePos.y)) {
                        menu=3;
                        screen=2;
                    }
                    else if (option4Bounds.contains(mousePos.x, mousePos.y)) {
                        menu=4;
                    }

                }
            }





        }


        window.display();
        window.clear();

    }

    TextureManager:: Clear();




    ///END OF SFML SECTION!!


    //it isn't allowing you to choose a different menu option
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

            graph.printConnections(from, window);
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
            //do we want to give them the option of using bfs or dfs?
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
