# Search Engine Project

## Overview  
This project is a lightweight search engine that indexes text files and allows users to perform keyword-based searches. It implements caching, a priority queue for ranking results, and a document graph for related searches.

## Features  
- Indexes documents from a directory  
- Supports keyword-based search with ranking  
- Implements caching for faster repeated searches  
- Uses a graph structure to track document relationships  
- Provides shortest path search between documents
- Can search for a given query on the internet
- Interactive easy-to-use GUI
- Based on hash tables for efficiency 

## Project Directory Structure
```plaintext
project_root/
├── Header Files/
│   ├── errorwindow.h
│   ├── graphwindow.h
│   ├── gui1.h
│   ├── querywindow.h
│   ├── searchengine.h
│   ├── webpagedisplay.h
├── Qt UI Files/
│   ├── errorwindow/
│   ├── graphwindow/
│   ├── gui1/
│   ├── querywindow/
│   ├── searchengine/
│   ├── webpagedisplay/
├── Sample Folder/
│   ├── datastructures.txt
│   ├── TechNotes/
│   │   ├── aboutgoogle.txt
│   │   ├── cameras.txt
│   │   ├── laptops.txt
│   │   ├── phones.txt
│   │   ├── FuturisticTech/
│   │   │   ├── vr.txt
│   ├── Folder1/
│   │   ├── textFold1.txt
│   │   ├── Folderin1/
│   │   │   ├── YeehawTextFile.txt
├── Sample Text Files/
│   ├── arti.txt
│   ├── LALA.txt
│   ├── school.txt
│   ├── TEXT.txt
│   ├── university.txt
├── Source Files/
│   ├── main.cpp
│   ├── errorwindow.cpp
│   ├── graphwindow.cpp
│   ├── gui1.cpp
│   ├── querywindow.cpp
│   ├── searchengine.cpp
│   ├── webpagedisplay.cpp
├── purple.png (background for the display)
├── CMakeLists and other related files
├── README.md
```




## Required Changes  
Before running the project, update the following paths in the code:  

### Modify the directory path for directory access 
In `searchEngine::store()`, update the `path` variable with the actual directory containing your documents.  
In `void WebpageDisplay::on_textBrowser_anchorClicked(const QUrl &url)`, update the `filepath` variable with the actual directory containing your documents. 


### Usage
#### 1. Index Documents
The program will traverse the given directory and index text documents.

#### 2. Search for a Word or Query
Enter a search term, and the system will return ranked results based on frequency.

#### 3. Cache Implementation
The program caches previous searches to improve response times.

#### 4. Graph-Based Search
The search engine uses a graph to determine relationships between documents, allowing shortest-path queries between documents.

##### License
This project is licensed under the MIT License.
