#include "searchengine.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>



// Reads the content of a file into a string
string searchEngine::readFileToString(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw runtime_error("Could not open file: " + filePath);
    }
    ostringstream sstream;
    sstream << inputFile.rdbuf();
    return sstream.str();
}

// Hash function for storing words
int searchEngine::hashFunction(string word)
{
    int code = 0;
    for (int i = 0; i < word.size(); i++)
    {
        char n = word[i];
        int c = static_cast<int>(n);
        code += (c * pow(2, i));
    }
    code = code % ::size;
    return code;
}

// Hash function for caching queries
int searchEngine::cacheHashFunction(string query) {
    int code = 0;
    for (size_t i = 0; i < query.size(); i++) {
        char n = query[i];
        int c = static_cast<int>(n);
        code += (c * pow(2, i));
    }
    code = code % cacheSize;
    return code;
}

// Converts a string to uppercase
void searchEngine::toUpper(string& str) {
    for (char& c : str) {
        c = toupper(c);
    }
    // Debug output
    cout << "Converted to upper: " << str << endl;
}

// Stores a search result in the cache
void searchEngine::storeInCache(string str, priority_queue<Node, vector<Node>, GreaterCount> q) {
    int index = cacheHashFunction(str);
    cacheNode* newnode = new cacheNode;
    newnode->query = str;
    newnode->searches = q;
    Cache[index].push_back(*newnode);
}

// Searches for a query in the cache
void searchEngine::searchInCache(string str, bool& found) {
    int index = cacheHashFunction(str);
    auto iter = Cache[index].begin();
    while (iter != Cache[index].end()) {
        if (str == iter->query) {
            cout << "FOUND IN CACHE" << endl;
            displayCachePriorityQueue(iter->searches);
            found = true;
            break;
        }
        iter++;
    }
}

void searchEngine::traverseDirectory(const string& path) {
    for (const auto& entry : directory_iterator(path)) {
        if (entry.is_directory() && entry.path().filename() == "build") {
            // Skip processing the "build" directory
            continue;
        }

        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string name = entry.path().filename().string();
            string fullPath = entry.path().string(); // Get the full path
            graphNode* newnode = new graphNode;
            newnode->document = name;
            newnode->docPath = fullPath; // Store the full path
            documentGraph.push_back(*newnode); // MAKES A GRAPH NODE
            cout<<newnode->document<<", "<<newnode->docPath<<endl;
        }

        if (entry.is_directory()) {
            // Recursively traverse subdirectories
            traverseDirectory(entry.path().string());
        }
    }
}

// Stores documents in the search engine
void searchEngine::store() {
    string path = "C:\\Users\\rijaa\\Desktop\\Sem 4 to be sorted\\projectnew\\projectnew"; // Relative path to project directory

    traverseDirectory(path);

    // Remove specific files if they exist
    for (auto temp = documentGraph.begin(); temp != documentGraph.end();) {
        if (temp->document == "CMakeLists.txt" || temp->document == "CMakeLists.txt.user") {
            temp = documentGraph.erase(temp);
        } else {
            ++temp;
        }
    }

    string substr = ".txt";
    for (graphNode &docNode : documentGraph) {
        try {
            string document = docNode.document;
            string filePath = docNode.docPath;
            string fileContents = readFileToString(filePath);

            fileContents.push_back('-');   // So our last word gets printed correctly

            bool conditionmet = false;
            string str = "";
            for (char c : fileContents) {
                if (c != ' ' && c != ',' && c != '-' && c != '\n') {
                    str += c;
                    continue;
                }

                if (str.size() > 0) {
                    if (str.back() == '.') {
                        str.pop_back();
                    }
                    if (search(str.begin(), str.end(), substr.begin(), substr.end()) != str.end()) {
                        edgeNode* newEdge = new edgeNode;
                        newEdge->edgeDoc = str;
                        docNode.edges.push_back(*newEdge);
                    }

                    toUpper(str);
                    int index = hashFunction(str);
                    typename list<Node>::iterator iter = array[index].begin();
                    while (iter != array[index].end()) {
                        if (iter->word == str && iter->docName == document) {
                            iter->count++;
                            conditionmet = true;
                            break;
                        }
                        iter++;
                    }
                    if (!conditionmet) {
                        Node* newnode = new Node;
                        newnode->count = 1;
                        newnode->docName = document;
                        newnode->word = str;
                        newnode->docPath = filePath;
                        array[index].push_back(*newnode);
                        cout << "Stored word: " << str << " in document: " << document << " at index: " << index << endl;
                    }
                    conditionmet = false;
                    str = "";
                }
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}


// Searches for a word in the stored documents
void searchEngine::searchWord(string tofind) {
    toUpper(tofind);
    int index = hashFunction(tofind);
    auto iter = array[index].begin();
    bool found = false;
    while (iter != array[index].end()) {
        if (iter->word == tofind) {
            searchesFound.push(*iter);
            found = true;
            // Debug output
            cout << "Found word: " << tofind << " in document: " << iter->docName << " with count: " << iter->count << endl;
        }
        iter++;
    }
    if (!found) {
        cout << "Word not found: " << tofind << endl;
    }
}

// Searches for a query with support for Boolean operators
void searchEngine::searchQuery(string query) {
    toUpper(query);

    bool foundInCache = false;
    searchInCache(query, foundInCache);
    if (foundInCache) {
        return;
    }
    cout << "NOT FOUND IN CACHE" << endl;

    vector<string> words;
    stringstream ss(query);
    string word;
    while (ss >> word) {
        toUpper(word); // Ensure each word is uppercase
        words.push_back(word);
    }

    cout << "\nSearch vector: ";
    for (const auto& element : words) {
        cout << element << " ";
    }
    cout << endl;

    vector<Node> resultsAnd;
    vector<Node> resultsOr;

    bool useAnd = false;
    bool useOr = false;

    bool andResult = true; // and logic is the default logic
    bool orResult = false;

    for (const auto& word : words) {
        if (word == "AND") {
            useAnd = true;
            andResult = true;
            orResult = false;
            continue;
        } else if (word == "OR") {
            useOr = true;
            andResult = false;
            orResult = true;
            continue;
        }

        vector<Node> tempResults = findNodesForWord(word);
        cout << "Results for word: " << word << " - " << tempResults.size() << " found." << endl;

        if (useAnd) {
            if (resultsAnd.empty()) {
                resultsAnd = tempResults;
            } else {
                resultsAnd = intersectResults(resultsAnd, tempResults);
            }
            useAnd = false;
        } else if (useOr) {
            resultsOr.insert(resultsOr.end(), tempResults.begin(), tempResults.end());
            useOr = false;
        } else {
            resultsOr.insert(resultsOr.end(), tempResults.begin(), tempResults.end());
            if (resultsAnd.empty()) {
                resultsAnd = tempResults;
            } else {
                resultsAnd = intersectResults(resultsAnd, tempResults);
            }
        }
    }

    vector<Node> finalResults;
    if (andResult) {
        finalResults = resultsAnd;
    } else {
        finalResults = resultsOr;
    }

    auto aggregatedResults = aggregateResults(finalResults);
    searchesFound = priorityQueueFromVector(aggregatedResults);

    cout << "Aggregated Results Size: " << aggregatedResults.size() << endl;
    if (!searchesFound.empty()) {
        storeInCache(query, searchesFound);
    }

    // Debug output
    cout << "Search results for query: " << query << endl;
    displayPriorityQueue();
}


// Displays the results from the cache
void searchEngine::displayCachePriorityQueue(priority_queue<Node, vector<Node>, GreaterCount> q) {
    while(!q.empty())
    {
        Node n = q.top();
        q.pop();
        n.displayNode();
    }
}

// Displays the search results
void searchEngine::displayPriorityQueue() {
    priority_queue<Node, vector<Node>, GreaterCount> temp = searchesFound;
    while(!temp.empty())
    {
        Node n = temp.top();
        temp.pop();
        n.displayNode();
    }
}

// Finds nodes for a given word
vector<Node> searchEngine::findNodesForWord(const string& word) {
    vector<Node> result;
    int index = hashFunction(word);
    for (const auto& node : array[index]) {
        if (node.word == word) {
            result.push_back(node);
        }
    }
    return result;
}

// Intersects two sets of search results
vector<Node> searchEngine::intersectResults(const vector<Node>& list1, const vector<Node>& list2) {
    vector<Node> result;
    unordered_map<string, Node> map;
    for (const auto& node : list1) {
        map[node.docName] = node;
    }
    for (const auto& node : list2) {
        if (map.find(node.docName) != map.end()) {
            result.push_back(node);
        }
    }
    return result;
}

// Converts a vector of nodes to a priority queue
priority_queue<Node, vector<Node>, GreaterCount> searchEngine::priorityQueueFromVector(const vector<Node>& vec) {
    priority_queue<Node, vector<Node>, GreaterCount> pq(vec.begin(), vec.end());
    return pq;
}

// Aggregates search results by summing the counts of identical nodes
vector<Node> searchEngine::aggregateResults(const vector<Node>& results) {
    unordered_map<string, Node> map;
    for (const auto& node : results) {
        if (map.find(node.docName) == map.end()) {
            map[node.docName] = node;
        } else {
            map[node.docName].count += node.count;
        }
    }
    vector<Node> aggregated;
    for (const auto& pair : map) {
        aggregated.push_back(pair.second);
    }
    return aggregated;}

bool searchEngine::searchFileByName(const std::string& fileName, std::string& fileContents) {
    for (const graphNode& documentNode : documentGraph) {
        if (documentNode.document == fileName) {
            string filePath = documentNode.docPath;
            fileContents = readFileToString(filePath);
            return true;
        }
    }
    return false;
}


string searchEngine::displayGraph()
{
    string s="";
    for (const graphNode& g : documentGraph)
    {
        s+=g.document+": ";
        cout << g.document << ": ";
        for (auto it = g.edges.begin(); it != g.edges.end(); ++it)
        {
            s+=it->edgeDoc;
            cout << it->edgeDoc;
            if (next(it) != g.edges.end()) {
                s+=", ";
                cout << ", ";
            }
        }
        s+="\n";
        cout << endl;
    }
    return s;
}


string searchEngine::retPath(string fileName)
{
    for(graphNode node: documentGraph)
    {
        if(node.document==fileName)
        {
            cout<<"yo: "<<node.docPath;
            return node.docPath;
        }
    }
    return "  ";
}

string searchEngine::dijkstra(string source, string destination) {
    unordered_map<string, int> dist;
    unordered_map<string, string> pred;  // To store the predecessors

    for (graphNode node : documentGraph) {
        dist[node.document] = numeric_limits<int>::max();
    }

    try{
    dist[source] = 0;
    dist[destination]= numeric_limits<int>::max();
    }
    catch(exception e)
    {
        return "Invalid Entry";
    }

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        string u = pq.top().second;
        pq.pop();

        for (edgeNode edge : getEdges(u)) {
            string v = edge.edgeDoc;
            int weight = edge.weight;

            // If there is a shorter path to v through u
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
                pred[v] = u;  // Update predecessor
            }
        }
    }

    // Reconstruct the shortest path from source to destination
    if (dist[destination] == numeric_limits<int>::max()) {
        cout << "No path from " << source << " to " << destination << endl;
        string s = "No path from "+source+" to "+destination;
        return s;
    }

    stack<string> path;
    string current = destination;
    while (current != source) {
        path.push(current);
        current = pred[current];
    }
    path.push(source);

    string s = "";
    cout << "Shortest path from " << source << " to " << destination << " is: ";
    while (!path.empty()) {
        cout << path.top();
        s = s+path.top();
        path.pop();
        if (!path.empty()){
            s = s+=" -> ";
            cout << " -> ";}
    }
    cout<<"\nString test: "<<s;
    cout << endl;
    cout << "Shortest distance: " << dist[destination] << endl;

    return s;
}

// Additional helper method to retrieve edges of a given vertex
list<edgeNode> searchEngine::getEdges(string vertex) {
    for (graphNode node : documentGraph) {
        if (node.document == vertex) {
            return node.edges;
        }
    }
    return list<edgeNode>();
}

