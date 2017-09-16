Wunner

A toy search engine which searches the web inside your terminal :p

**Development started, don't test it, nothing works as of now**

TODO checklist:

|* Add simple main() tests for each module  
|* For terminal based, show appropriate outputs at each step  
|* Colors etc to be added, beautify the output  
|* Add support for complete matching queries  
|* Decide which option is better: i> Terminal based: simpler, provide parameters then (preferred) ii> Web UI: complex (todo laaaaater)   
|* Reduce number of files used, prefer to keep things in memory wherever possible, think of database  
|* Better ways to serialize objects  
|* Improve modularity  
|* Dynamic linking  
|* Implement interaction with robots.txt in crawler  
|* Provide minimum reqd versions of boost, cmake, compiler etc  
|* Add standard readme  

Temporary steps to build   

Compiler (ideally gcc/clang) must support C++14   
1. `cd Wunner` from where you cloned/downloaded it   

2. for linux, reqd->boost, wget, ar  
   a> `chmod +x wunner_build.sh`  
   b> `./wunner_build.sh`   
note: this defaults to g++ compiler, to use another append compiler name to it (eg: `./wunner_build.sh clang++`)  

OR

2. requirements: make, boost, wget and cmake  
   a> `mkdir -p build && cd build && cmake .. && make`  

3. ultimately run `wunner_search` from root of directory Wunner (either directly `./build/bin/wunner_search` or do `export PATH=$PATH:${PWD}/build/bin` before)   

note: to start the search engine afresh (crawling, building index etc again), add option `-f` or `--fresh` -> `wunner_search -f`   

OR

2. using docker-> install docker on linux/mac/win   
   a> `docker build -t wunner .`  
   b> `docker run -v ${PWD}:/tmp wunner wunner_search` with su privileges

