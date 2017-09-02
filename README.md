Wunner

A toy search engine which searches the web inside your terminal :p

Development started, don't test it, nothing works as of now

TODO checklist:

|* Add simple main() tests for each module  
|* For terminal based, show appropriate outputs at each step  
|* Colors etc to be added, beautify the output  
|* Add support for complete matching queries  
|* Decide which option is better: i> Terminal based: simpler, provide parameters then (preferred) ii> Web UI: complex (todo laaaaater)   
|* Reduce number of files used, prefer to keep things in memory wherever possible, think of database  
|* Resolve dependencies  
|* Better ways to serialize objects  
|* Dynamic linking  
|* Improve modularity  
|* Implement interaction with robots.txt in crawler  
|* Add standard readme  

temporary steps to build   

1. `cd Wunner` from where you cloned/downloaded it   

2a. requirements: boost, boost regex, wget and cmake  
2b. use cmake for generating binary, with -DBUILD_STATIC=1 option for static linking  
eg commands-> `mkdir -p build && cd build && cmake -DBUILD_STATIC=1 .. && make`  

OR

2. using docker-> install docker on linux   
`docker build -t wunner docker/` and `docker run -v ${PWD}:/tmp wunner` with su privileges 

3. ultimately run `wunner_search` from root of directory Wunner

