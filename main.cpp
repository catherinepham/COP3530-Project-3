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
    map<Person, Person> mp;

public:
    void insertEdge(Person from, Person to);
    vector<string> DepthFirstSearch(Person from, string company);
    vector<string> BreadthFirstSearch(Person from, string company);
    bool insertConnection(Person from, Person to);
    void printConnections(Person from, sf:: RenderWindow &window);
    Person BFSHelper(Person from, string company);
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


Person Graph::BFSHelper(Person from, string company) {
    set<Person> visited;
    queue<Person> q;

    visited.insert(from);
    q.push(from);

    while(!q.empty()){
        Person person1 = q.front();
        q.pop();

        if(person1.company == company) {
            return person1;
        }

        vector<Person> connections = graph[person1];
        for (Person v: connections) {
            if(visited.count(v)==0) {
                mp[v] = person1;
                if(v.company == company) {
                    return v;
                }
                visited.insert(v);
                q.push(v);
            }
        }
    }
    Person empty;
    return empty;
}

vector<string> Graph::BreadthFirstSearch(Person from, string company) {
    Person to = BFSHelper(from, company);
    vector<string> connectionsPath;

    Person prev;
    Person curr = to;
    connectionsPath.push_back(to.name);
    while (prev.name!=from.name && prev.company!=from.company) {
        prev = mp[curr];
        connectionsPath.push_back(prev.name);
        curr = prev;
        if(curr.name=="" || prev.name=="")
        {
            break;
        }
    }

    return connectionsPath;
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

        vector<Person> connections = graph[person1];
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

    Graph graph;
    vector<Person> people;
    string filePath= "MassiveConnections.csv";
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

   // sf:: Sprite otherNetwork(TextureManager::GetTexture("otherNetwork"));
    //otherNetwork.setScale(0.6,0.6);
    //otherNetwork.setPosition(500, 600);

    sf:: Sprite question(TextureManager::GetTexture("question"));
    question.setPosition(465, 10);

    sf:: Sprite insertQ(TextureManager::GetTexture("insertQ"));
    insertQ.setPosition(550, 10);

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

    sf:: Sprite nameInput2(TextureManager::GetTexture("nameInput"));
    nameInput2.setPosition(180,1070);
    sf:: Sprite companyInput2(TextureManager::GetTexture("companyInput"));
    companyInput2.setPosition(150,1270);

    sf:: Sprite titleInput(TextureManager::GetTexture("titleInput"));
    titleInput.setPosition(320,50);

    sf:: Sprite failed(TextureManager::GetTexture("failed"));
    failed.setPosition(420,1200);

    sf:: Sprite success(TextureManager::GetTexture("success"));
    success.setPosition(530,1200);

    sf:: Sprite companyAsk(TextureManager::GetTexture("companyAsk"));
    companyAsk.setPosition(450,700);

    sf:: Sprite pathQ(TextureManager::GetTexture("pathQ"));
    pathQ.setPosition(410, 40);

    sf:: Sprite border(TextureManager::GetTexture("border"));
    border.setPosition(510,65);

    int screen=1;
    int menu=1;
    int insertResult=0;
    Person from;

    sf::String userInputname;
    sf::String userInputcompany;
    sf::Font font;
    sf::Text nameText("", font, 75);
    nameText.setFont(font);
    nameText.setPosition(575,610);
    nameText.setColor(sf::Color::White);
    sf::Text companyText("", font, 75);
    companyText.setFont(font);
    companyText.setPosition(625,930);
    companyText.setColor(sf::Color::White);

    bool nameInputted = false;
    bool companyInputted = false;
    bool toInsert = false;
    int insert = 0;

    sf::String networkName;
    sf::String networkCompany;
    sf::Text netNameText("", font, 75);
    netNameText.setFont(font);
    netNameText.setPosition(600,1110);
    netNameText.setColor(sf::Color::White);
    sf::Text netCompanyText("", font, 75);
    netCompanyText.setFont(font);
    netCompanyText.setPosition(740,1330);
    netCompanyText.setColor(sf::Color::White);

    bool netNameInputted = false;
    bool netCompanyInputted = false;

    sf::String userInputCompany2;
    sf::String userText2;
    sf::Text companyText2("", font, 75);
    companyText2.setFont(font);
    companyText2.setPosition(495,850);
    companyText2.setColor(sf::Color::White);
    bool finishedComp= false;




    while(window.isOpen())
    {
        if(screen==1)
        {
            userInputname="";
            userInputcompany="";
            userInputCompany2="";
            userText2="";
            nameText.setString(userInputname);
            companyText.setString(userInputcompany);
            companyText2.setString(userInputCompany2);

            bool nameInputted = false;
            bool companyInputted = false;
            bool toInsert = false;
            bool finishedComp= false;
            int insert = 0;

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
            window.draw(nameInput2);
            window.draw(companyInput2);
            window.draw(netCompanyText);
            window.draw(netNameText);

            if(menu==2)
            {
                window.draw(insertQ);
            }
            else
            {
                window.draw(question);
            }
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
            window.draw(menuButton);
            window.draw(nameText);
            window.draw(companyText);

            if (toInsert) {
                bool insertVal = graph.insertConnection(from, Person(userInputname,userInputcompany));
                if (insertVal)
                    insert = 1;
                else
                    insert = 2;
                toInsert = false;
            }

            if(insert==1)
            {
                window.draw(success);
            }
            else if(insert==2)
            {
                window.draw(failed);
            }
        }
        else if(screen==5)
        {
            window.draw(companyAsk);
            window.draw(pathQ);
            window.draw(companyText2);

            if (finishedComp== true && menu==3)
            {
                screen=6;
            }
            else if(finishedComp== true && menu==4)
            {
                screen=7;
            }


        }
        else if(screen==6)
        {
            //breadth first
            window.clear();
            string company= userInputCompany2;

            sf::Font font;
            font.loadFromFile("Font/arial.ttf");
            if(!font.loadFromFile("Font/arial.ttf"))
            {
                cout<< "ERROR IN FONT" << endl;
            }

            window.draw(border);
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(70);
            text.setFillColor(sf::Color::White);
            text.setString("The BFS path from you to "+ company+ " is:");
            text.setPosition(560,100);
            window.draw(text);
            window.draw(menuButton);

            cout<< from.name<< " "<< from.company<< endl;
            company.erase(company.size()-1);


            vector<string> connections = graph.BreadthFirstSearch(from, company);
            cout << "The SHORTEST path from you to " << company << ": " << endl;
            for (auto i: connections) {
                cout << i << endl;
            }

            screen=1;

        }
        else if(screen==7)
        {
            //depth first
            window.draw(menuButton);
            string company=userInputCompany2;
            vector<string> connections = graph.DepthFirstSearch(from, company);
            cout << "Your path of connections to " << company << ": " << endl;
            for (auto i: connections) {
                cout << i << endl;
            }

            /*
            sf::Font font;
            font.loadFromFile("Font/arial.ttf");

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);


            string message= graph[from].at(i).name+"-- "+graph[from].at(i).company;
            text.setString(message);
            text.setPosition(j,m*22);
            window.draw(text);
*/
        }

        font.loadFromFile("Font/arial.ttf");
        if(!font.loadFromFile("Font/arial.ttf"))
        {
            cout<< "ERROR IN FONT" << endl;
        }

        sf:: Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type==sf::Event::TextEntered) {
                if (event.text.unicode < 128 && screen==2)
                {
                    if (event.text.unicode == 10 && !netNameInputted) {
                        netNameInputted = true;
                    }
                    else if(event.text.unicode == 10 && netNameInputted && !netCompanyInputted) {
                        netCompanyInputted = true;
                    }
                    else if (!netNameInputted && !netCompanyInputted) {
                        networkName +=event.text.unicode;
                        netNameText.setString(networkName);
                    }
                    else if (netNameInputted && !netCompanyInputted) {
                        networkCompany +=event.text.unicode;
                        netCompanyText.setString(networkCompany);
                    }
                    else if (netNameInputted && netCompanyInputted){
                        from = Person(networkName, networkCompany);
                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }
                }
                if (event.text.unicode < 128 && screen==4) {
                    if (event.text.unicode == 10 && !nameInputted) {
                        nameInputted = true;
                    }
                    else if(event.text.unicode == 10 && nameInputted && !companyInputted) {
                        companyInputted = true;
                    }
                    else if (!nameInputted && !companyInputted) {
                        userInputname +=event.text.unicode;
                        nameText.setString(userInputname);
                    }
                    else if (nameInputted && !companyInputted) {
                        userInputcompany +=event.text.unicode;
                        companyText.setString(userInputcompany);
                    }
                    else if (nameInputted && companyInputted){
                        toInsert = true;
                    }
                }
                if (event.text.unicode < 128 && screen==5)
                {
                    //need bool outside of loop, return true if hit enter
                    userInputCompany2 +=event.text.unicode;
                    companyText2.setString(userInputCompany2);
                    if (event.text.unicode == 10)
                    {
                        finishedComp= true;
                    }
                }

            }

            if (event.type== sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    auto boundsCamille = camille.getGlobalBounds();
                    if (boundsCamille.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Camille Eyman";
                        from.company = "UF Center for Arts in Medicine";

                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    auto boundsNathalie = nathalie.getGlobalBounds();
                    if (boundsNathalie.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Nathalie Leave";
                        from.company = "Microsoft";

                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    auto boundsCathy = cathy.getGlobalBounds();
                    if (boundsCathy.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Catherine Pham";
                        from.company = "University of Florida";
                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    auto boundsMenu = menuButton.getGlobalBounds();
                    if (boundsMenu.contains(mousePos.x, mousePos.y)) {
                        screen = 1;
                        insertResult = 0;
                    }

                    auto option1Bounds = option1.getGlobalBounds();
                    auto option2Bounds = option2.getGlobalBounds();
                    auto option3Bounds = option3.getGlobalBounds();
                    auto option4Bounds = option4.getGlobalBounds();

                    if (option1Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 1;
                        screen = 2;
                    } else if (option2Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 2;
                        screen = 2;
                    } else if (option3Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 3;
                        screen = 2;
                    } else if (option4Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 4;
                        screen = 2;
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
#include <list>
using namespace std;

//Person class to store name and company of vertex
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

    //overrides the ability to make a key of map a complex object
    bool operator<(const Person &ob) const
    {
        return name < ob.name || (name == ob.name && company < ob.company);
    }


};

//graph class to store graph structure
class Graph
{
private:
    //the graph itself is a map
    map<Person, vector<Person>> graph;
    //mp is a helper structure for the BFS function
    map<Person, Person> mp;

public:
    //inserts Edge into graph
    void insertEdge(Person from, Person to);
    //depth first
    vector<string> DepthFirstSearch(Person from, string company);
    //breadth first
    vector<string> BreadthFirstSearch(Person from, string company);
    Person BFSHelper(Person from, string company);
    //inserts a new connection
    bool insertConnection(Person from, Person to);
    //prints all of the connections
    void printConnections(Person from, sf:: RenderWindow &window);
};


void Graph::printConnections(Person from, sf:: RenderWindow &window)
{
    //download the fonts to output text to the screen
    sf::Font font;
    font.loadFromFile("Font/arial.ttf");
    if(!font.loadFromFile("Font/arial.ttf"))
    {
        cout<< "ERROR IN FONT" << endl;
    }

    //create text and assign font
    sf::Text text;
    text.setFont(font);

    //if graph is 0, the person has no connections
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

    //string message= "This person doesn't exist in the network";
    //text.setString(message);

    //makes text right color and size (small and white)
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);


    //variables to assist with the placement of the names/companies on the screen
    int j=0;
    int m=0;

    //prints all connections in loop
    for(int i=0; i<graph[from].size(); i++)
    {
        string message= graph[from].at(i).name+"-- "+graph[from].at(i).company;
        text.setString(message);
        text.setPosition(j,m*22);
        window.draw(text);
        //this moves to the next oclumn to output
        if(i!=0 && i%70==0)
        {
            j+=700;
            m=0;
        }
        m++;
    }
    return;
}


//BFS helper function pushes the valid connections to a map (which stores vertex, and previous vertex)
//this helps us so that we are not printing every single vertex that we touch in the path, but rather only the "connectors"
//BFS uses this function
Person Graph::BFSHelper(Person from, string company) {
    set<Person> visited;
    queue<Person> q;

    //visited ensures that we don't go into any loops
    visited.insert(from);

    //queue helps us BFS
    q.push(from);

    //while we still have untouched vertices in the graph
    while(!q.empty()){
        Person person1 = q.front();
        q.pop();

        if(person1.company == company) {
            return person1;
        }

        vector<Person> connections = graph[person1];
        for (Person v: connections) {
            if(visited.count(v)==0) {
                mp[v] = person1;
                if(v.company == company) {
                    return v;
                }
                visited.insert(v);
                q.push(v);
            }
        }
    }
    Person empty;
    return empty;
}

//BFS uses the Helper BFS funciton to only print the values that are important to us to find the lowest degrees of seperation
vector<string> Graph::BreadthFirstSearch(Person from, string company) {
    Person to = BFSHelper(from, company);

    //if we didn't find anyone who worked there, return an empty vector
    if(to.name=="")
    {
        vector<string> connectionsPath;
        return connectionsPath;
    }
    vector<string> connectionsPath;

    //otherwise only add the important (prev) values to the vector
    Person prev;
    Person curr = to;
    connectionsPath.push_back(to.name);
    while (prev.name!=from.name && prev.company!=from.company) {
        prev = mp[curr];
        connectionsPath.push_back(prev.name);
        curr = prev;
        if(curr.name=="" || prev.name=="")
        {
            break;
            //cout<< "reaching end";
        }
    }

    return connectionsPath;
}

//insertEdge() adds a new edge between the from and to vertex.
void Graph::insertEdge(Person from, Person to)
{
    //has to be a doubly directed graph because the connections all go 2 ways
    //check for connections inserted twice because of doubly directed
    graph[from].push_back(to);
    graph[to].push_back(from);
}


//DFS returns every node we touch
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

        //should be graph[person1]
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



//this allows us to insert a new connection into the graph
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

    //these values are for if we want to use this program in the console of CLion
    int lines = 7;
    int menuNum = 0;

    //create vector of people and graph
    Graph graph;
    vector<Person> people;

    //load the connections
    string filePath= "NewMassiveConnections.csv";
    ifstream File(filePath);


    if(File.is_open())
    {
        string lineFromFile;
        getline(File, lineFromFile);

        //get every line of the CSV file
        while(getline(File, lineFromFile))
        {
            istringstream stream(lineFromFile); //stream of data coming from a string

            //name, company, connection first, connection last, connection company
            //those are the format headings of the CSV file
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




            //concatinate into 1 connection name
            string connectionName= connectionFirst+ " " + connectionLast;
            //erase the newline char
            connectionCompany= connectionCompany.erase(connectionCompany.size() - 1);

            //if there is a space at the end of any names, delete
            if(connectionName[connectionName.size()-1]== ' ')
            {
                connectionName= connectionName.erase(connectionName.size()-1);
            }
            if(name[name.size()-1]== ' ')
            {
                name= name.erase(name.size()-1);
            }


            //checks if we already have this person in the graph
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

            Person person1= Person(name,company);

            //if they aren;t already added, then push them into the people vect
            if(!found1) {
                people.push_back(person1);
            }

            Person connection= Person(connectionName, connectionCompany);

            //if they aren't already added, then push them into the people vect
            if(!found2)
            {
                people.push_back(connection);
            }

            //insert the edge
            graph.insertEdge(person1, connection);
        }
    }


    ///SFML SECION!!!

    //create window to desired size
    sf::RenderWindow window(sf::VideoMode(2700, 1600), "Girl Boss, Gatekeep, Graph Functions: Linkedin Connection Analyzer");

    //ADD ALL SPRITES AND POSITION THEM

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
    question.setPosition(465, 10);

    sf:: Sprite insertQ(TextureManager::GetTexture("insertQ"));
    insertQ.setPosition(550, 10);

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

    sf:: Sprite companyAsk(TextureManager::GetTexture("companyAsk"));
    companyAsk.setPosition(450,700);

    sf:: Sprite pathQ(TextureManager::GetTexture("pathQ"));
    pathQ.setPosition(410, 40);

    sf:: Sprite border(TextureManager::GetTexture("border"));
    border.setPosition(510,65);

    sf:: Sprite nameInput2(TextureManager::GetTexture("nameInput"));
    nameInput2.setPosition(180,1070);
    sf:: Sprite companyInput2(TextureManager::GetTexture("companyInput"));
    companyInput2.setPosition(150,1270);

    sf:: Sprite notFound(TextureManager::GetTexture("notFound"));
    notFound.setPosition(830,450);

    //All sprites have been added, scaled, and positioned


    //our screen is initialized to start on welcome screen
    int screen=1;
    int menu=1;
    int insertResult=0;

    //initial person object to be altered with the commands
    Person from;

    //these values are all allowing the user to type onto the screen
    sf::String userInputname;
    sf::String userInputcompany;
    sf::Font font;
    sf::Text nameText("", font, 75);
    nameText.setFont(font);
    nameText.setPosition(575,610);
    nameText.setColor(sf::Color::White);
    sf::Text companyText("", font, 75);
    companyText.setFont(font);
    companyText.setPosition(625,930);
    companyText.setColor(sf::Color::White);

    bool nameInputted = false;
    bool companyInputted = false;
    bool toInsert = false;
    int insert = 0;

    sf::String networkName;
    sf::String networkCompany;
    sf::Text netNameText("", font, 75);
    netNameText.setFont(font);
    netNameText.setPosition(600,1110);
    netNameText.setColor(sf::Color::White);
    sf::Text netCompanyText("", font, 75);
    netCompanyText.setFont(font);
    netCompanyText.setPosition(740,1330);
    netCompanyText.setColor(sf::Color::White);

    bool netNameInputted = false;
    bool netCompanyInputted = false;


    sf::String userInputCompany2;
    sf::String userText2;
    sf::Text companyText2("", font, 75);
    companyText2.setFont(font);
    companyText2.setPosition(495,850);
    companyText2.setColor(sf::Color::White);
    bool finishedComp= false;


    //all above are helping user type onto screen


    //these are the grahpics
    while(window.isOpen())
    {
        //welcome screen
        if(screen==1)
        {

            userInputname="";
            userInputcompany="";
            userInputCompany2="";
            userText2="";
            nameText.setString(userInputname);
            companyText.setString(userInputcompany);
            companyText2.setString(userInputCompany2);

            bool nameInputted = false;
            bool companyInputted = false;
            bool toInsert = false;
            bool finishedComp= false;
            int insert = 0;

            //draws the sprites for the welcome screen
            window.draw(title);
            window.draw(directions);
            window.draw(option1);
            window.draw(option2);
            window.draw(option3);
            window.draw(option4);
        }

        //chose the person's network screen (with the bitmojis)
        else if(screen==2)
        {
            //draw sprites (bitmojis)
            window.draw(camille);
            window.draw(nathalie);
            window.draw(cathy);
            window.draw(camilleName);
            window.draw(nathalieName);
            window.draw(cathyName);

            //allowes them to input their names
            window.draw(nameInput2);
            window.draw(companyInput2);
            window.draw(netCompanyText);
            window.draw(netNameText);


            //if we are inserting, have a different promp than the other times
            if(menu==2)
            {
                window.draw(insertQ);
            }
            else
            {
                window.draw(question);
            }
        }

        //print connections screen
        else if(screen==3)
        {
            //prints whole network
            graph.printConnections(from, window);
            //draws menu button
            window.draw(menuButton);
        }

        //insert new person screen
        else if(screen==4)
        {
            //draws sprites
            window.draw(nameInput);
            window.draw(companyInput);
            window.draw(titleInput);
            window.draw(menuButton);
            window.draw(nameText);
            window.draw(companyText);

            //if they typed something that will work, then insert the connection
            if (toInsert) {
                bool insertVal = graph.insertConnection(from, Person(userInputname,userInputcompany));
                //if it worked
                if (insertVal)
                    insert = 1;
                //if it didn't work (they were already in the graph)
                else
                    insert = 2;
                toInsert = false;
            }

            //draw success sprite
            if(insert==1)
            {
                window.draw(success);
            }

            //draw failed sprite
            else if(insert==2)
            {
                window.draw(failed);
            }
        }

        //company to find (BFS/DFS) screen
        else if(screen==5)
        {
            //draw sprites
            window.draw(companyAsk);
            window.draw(pathQ);
            window.draw(companyText2);

            //if they typed a company correctly and wanted BFS
            if (finishedComp== true && menu==3)
            {
                //change to screen 6
                screen=6;
            }

            //if they typed a company correctly and wanted DFS
            else if(finishedComp== true && menu==4)
            {
                //change to screen 7
                screen=7;
            }

        }

        //BFS output screen
        else if(screen==6)
        {
            //breadth first
            window.clear();

            string company= userInputCompany2;

            //output message about their path to a company
            sf::Font font;
            font.loadFromFile("Font/arial.ttf");
            if(!font.loadFromFile("Font/arial.ttf"))
            {
                cout<< "ERROR IN FONT" << endl;
            }

            //draw sprites
            window.draw(border);
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(70);
            text.setFillColor(sf::Color::White);
            text.setString("The shortest path from you to "+ company+ " is:");
            text.setPosition(560,100);
            window.draw(text);
            window.draw(menuButton);

            company.erase(company.size()-1);


            vector<string> connections = graph.BreadthFirstSearch(from, company);

            sf::Text text1;
            text1.setFont(font);
            text1.setCharacterSize(90);
            text1.setFillColor(sf::Color::Yellow);
            int j=400;

            //output to screen their path
            for (auto i: connections) {

                string message= i;
                text1.setString(message);
                text1.setPosition(900,j);
                window.draw(text1);
                j=j+240;
            }

            //if not found, output specific message via sprite
            if(connections.size()==0)
            {
                window.draw(notFound);
            }

            //screen=1;

        }

        //DFS screen
        else if(screen==7)
        {
            //depth first
            window.draw(menuButton);
            string company=userInputCompany2;
            company.erase(company.size()-1);
            vector<string> connections= graph.DepthFirstSearch(from, company);


            sf::Text text1;
            text1.setFont(font);
            text1.setCharacterSize(20);
            text1.setFillColor(sf::Color::Yellow);
            int j=30;
            int m=10;
            int k=0;

            //output to screen path of DFS
            for (auto i: connections) {

                string message= i;
                text1.setString(message);
                text1.setPosition(m,j);
                window.draw(text1);
                j=j+22;
                k++;
                if(k%70==0 && k!=0)
                {
                    m+=250;
                    j=30;
                }
            }
        }

        font.loadFromFile("Font/arial.ttf");
        if(!font.loadFromFile("Font/arial.ttf"))
        {
            cout<< "ERROR IN FONT" << endl;
        }

        sf:: Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            //all the screens where they type an input
            if (event.type==sf::Event::TextEntered) {
                //if it is on screen 2, it is them wanting to look at someoen's graph who isn;t one of us
                if (event.text.unicode < 128 && screen==2)
                {
                    //they hit enter, move to company block
                    if (event.text.unicode == 10 && !netNameInputted) {
                        netNameInputted = true;
                    }
                    //they hit enter and want to move to next string
                    else if(event.text.unicode == 10 && netNameInputted && !netCompanyInputted) {
                        netCompanyInputted = true;
                    }
                    else if (!netNameInputted && !netCompanyInputted) {
                        networkName +=event.text.unicode;
                        netNameText.setString(networkName);
                    }
                    else if (netNameInputted && !netCompanyInputted) {
                        networkCompany +=event.text.unicode;
                        netCompanyText.setString(networkCompany);
                    }

                    //move to next screen (which is decided by the current menu option)
                    else if (netNameInputted && netCompanyInputted){
                        from = Person(networkName, networkCompany);
                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }
                }

                //if we are on screen 4(want to input name and company to insert the connection)
                else if (event.text.unicode < 128 && screen==4) {
                    if (event.text.unicode == 10 && !nameInputted) {
                        nameInputted = true;
                    }
                    else if(event.text.unicode == 10 && nameInputted && !companyInputted) {
                        companyInputted = true;
                    }
                    else if (!nameInputted && !companyInputted) {
                        userInputname +=event.text.unicode;
                        nameText.setString(userInputname);
                    }
                    else if (nameInputted && !companyInputted) {
                        userInputcompany +=event.text.unicode;
                        companyText.setString(userInputcompany);
                    }

                    //if we have both values completed
                    else if (nameInputted && companyInputted){
                        toInsert = true;
                    }

                }

                //we are on screen 5, we want to insert company name to search for for BFS DFS
                else if (event.text.unicode < 128 && screen==5)
                {
                    //need bool outside of loop, return true if hit enter
                    userInputCompany2 +=event.text.unicode;
                    companyText2.setString(userInputCompany2);
                    //hit enter, done with screen
                    if (event.text.unicode == 10)
                    {
                        finishedComp= true;
                    }
                }
            }

            //if they pressed a button
            if (event.type== sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    //clicked on Camille
                    auto boundsCamille = camille.getGlobalBounds();
                    if (boundsCamille.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Camille Eyman";
                        from.company = "UF Center for Arts in Medicine";

                        //depends on menu option
                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    //clicked on Nathalie
                    auto boundsNathalie = nathalie.getGlobalBounds();
                    if (boundsNathalie.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Nathalie Leave";
                        from.company = "Microsoft";

                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    //clicked on Cathy
                    auto boundsCathy = cathy.getGlobalBounds();
                    if (boundsCathy.contains(mousePos.x, mousePos.y) && screen==2) {
                        from.name = "Catherine Pham";
                        from.company = "University of Florida";
                        if (menu == 1) {
                            screen = 3;
                        } else if (menu == 2) {
                            screen = 4;
                        } else {
                            screen = 5;
                        }
                    }

                    //clicked on Main Menu
                    auto boundsMenu = menuButton.getGlobalBounds();
                    if (boundsMenu.contains(mousePos.x, mousePos.y)) {
                        screen = 1;
                        insertResult = 0;
                        userInputname="";
                        nameText.setString("");
                        userInputcompany="";
                        companyText.setString("");
                        networkName="";
                        netNameText.setString("");
                        networkCompany="";
                        netCompanyText.setString("");
                        userInputCompany2="";
                        companyText2.setString("");
                    }

                    //clicking on any of the menu options on welcome screen
                    auto option1Bounds = option1.getGlobalBounds();
                    auto option2Bounds = option2.getGlobalBounds();
                    auto option3Bounds = option3.getGlobalBounds();
                    auto option4Bounds = option4.getGlobalBounds();

                    if (option1Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 1;
                        screen = 2;
                    } else if (option2Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 2;
                        screen = 2;
                    } else if (option3Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 3;
                        screen = 2;
                    } else if (option4Bounds.contains(mousePos.x, mousePos.y) && screen==1) {
                        menu = 4;
                        screen = 2;
                    }

                }
            }
        }


        //displays and clears the screen every iteration
        window.display();
        window.clear();

    }

    TextureManager:: Clear();




    ///END OF SFML SECTION!!

//THIS SECTION IS IF YOU WANTED TO RUN IN CONSOLE
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
