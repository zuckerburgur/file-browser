#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <iostream>
#include <string>
#include <filesystem>
#include <list>
#include <fstream>
#include <sstream>
#include <cctype>
#include <queue>
#include <vector>
#include <cmath>
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <stack>

using namespace std;
using namespace std::filesystem;

struct Node{
    string docName;
    string docPath;
    string word;
    int count;

    void displayNode()
    {
        cout<<"Word: "<<word<<"\nDoc Name: "<<docName<<"\nCount: "<<count<<endl<<endl;
    }
};

struct GreaterCount {
    bool operator()(const Node& n1, const Node& n2) {
        // Higher count has higher priority
        return n1.count < n2.count;
    }
};


struct cacheNode{
    string query;
    priority_queue<Node,vector<Node>,GreaterCount> searches;
};


struct edgeNode{
    string edgeDoc;
    int weight = 1;
};

struct graphNode{
    string document;
    string docPath;
    list<edgeNode> edges;
};


const int size = 500;
const int cacheSize = 100;

class searchEngine {
private:
    //list<string> documents;

    list<Node> array[::size];
    list<cacheNode> Cache[::cacheSize];

    string readFileToString(const string& filePath);
    int hashFunction(string word);
    int cacheHashFunction(string query);
    void toUpper(string& str);

    vector<Node> findNodesForWord(const string& word);
    vector<Node> intersectResults(const vector<Node>& list1, const vector<Node>& list2);
    priority_queue<Node, vector<Node>, GreaterCount> priorityQueueFromVector(const vector<Node>& vec);

public:
    list<graphNode> documentGraph;
    priority_queue<Node, vector<Node>, GreaterCount> searchesFound;

    void storeInCache(string str, priority_queue<Node, vector<Node>, GreaterCount> q);
    void searchInCache(string str, bool& found);
    void store();
    void searchWord(string tofind);
    void searchQuery(string query);
    void traverseDirectory(const string& path);
    void displayCachePriorityQueue(priority_queue<Node, vector<Node>, GreaterCount> q);
    void displayPriorityQueue();
    vector<Node> aggregateResults(const vector<Node>& results);
    bool searchFileByName(const std::string& fileName, std::string& fileContents);
    string displayGraph();
    string dijkstra(string source, string destination);
    list<edgeNode> getEdges(string vertex);
    string retPath(string fileName);
};

#endif // SEARCHENGINE_H
