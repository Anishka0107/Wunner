# Wunner

A toy search engine that searches the web inside your terminal :p

## Features
* Implemented in C++14.
* Crawls webpages progressively starting from seed URL(s).
* Parses the documents and the query, trying to generate more appropriate results.
* Builds an index (hash map) for the parsed documents.
* The crawled documents and index are refreshed periodically.
* Autocompletes query using a trie, based on most recently asked queries.
* Maintains two threads, to allow refreshing the index and querying simultaneuosly.
* Generates most relevant results in order ranked on the basis of harmonic mean of PageRank (to get the importance of webpage) and Okapi BM25 (to get query-based result) algorithm ranks.
* Provides query suggestions (only when the input query does not generate any results), on the basis of common incorrect and correct words. Ranks them using n-gram algorithm and edit-distance DP to compare two strings.

## Steps to Run
Command to run : `wunner_search` (make sure your PWD is the project's root directory)   
Add option `-f` or `--fresh` as in `wunner_search -f` to start the search engine afresh (i.e., crawling and indexing again)  
- After indexing gets completed, simply type your query and hit <kbd>Enter</kbd> to start searching  
- To use autocomplete, press <kbd>Ctrl+G</kbd> while typing query and then type the desired result's number to complete the query (it's not of relevance until a web UI is developed)  

## Steps to Build
1. Clone (`git clone https://github.com/Anishka0107/Wunner.git`) or download this repository  
2. `cd Wunner` from where it was cloned/downloaded  

#### Build (tested on Linux)
* Requirements : GCC (5.0 & above) / Clang (3.4 & above), Boost, Wget  
3. Two options :  
    1. Requires `ar` :  
        1. Run `chmod +x wunner_build.sh`  
        2. Run `./wunner_build.sh` (note that this defaults to g++ compiler; append compiler name to use other, eg: `./wunner_build.sh clang++`) 
    2. Requires `cmake` and `make`:  
	    1. Run `mkdir -p build && cd build && cmake .. && make -j$(nproc)`  
4. Ultimately run `wunner_search` (either directly `./build/bin/wunner_search` or do `export PATH=$PATH:${PWD}/build/bin` before)  

#### Docker based (for Linux/Windows/OS-X)
3. Set up [Docker](https://www.docker.com/) on your system (need root priviledges for docker commands)  
4. Build the image using `docker build -t wunner .`  
5. Run using `docker run -v ${PWD}:/tmp wunner wunner_search` (append wunner_search options if required)   

## TODO checklist:

- [ ] Add simple main() tests for each module  
- [ ] For terminal based, show appropriate outputs at each step  
- [ ] Add colours beautify the output  
- [ ] Command line options for `res` files  
- [ ] Add support for complete matching queries  
- [ ] Add support for relative URLs on webpage  
- [ ] Implement interaction with robots.txt in crawler  
- [ ] Build a web UI  
- [ ] Database instead of files to store objects  
- [ ] Dynamic linking in build  

#### Resources
* Crawler Seed URLs ->    
* Erroneous Words ->   
* List of Stop Words -> https://www.webconfs.com/stop-words.php  
