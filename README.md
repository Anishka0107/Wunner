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
|* Improve modularity  
|* Implement interaction with robots.txt in crawler  
|* Add standard readme  

temporary steps to build   

1. `cd Wunner` from where you cloned/downloaded it   

2. a> requirements: boost, wget and cmake  
2. b> use cmake for generating binary  
eg commands-> `mkdir -p build && cd build && cmake .. && make`  

3. ultimately run `wunner_search` from root of directory Wunner

OR

2. using docker-> install docker on linux   
   a> `docker build -t wunner .`  
   b> `docker run wunner wunner_search` with su privileges, from any directory

