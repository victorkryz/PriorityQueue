# PriorityQueue

Modelling of client/sever communications based on priority queue in a single application with command line interface.


### Usage:
-------------------------------------------------------------------------

#### Command line arguments:

```
-c, --clients    clients number (not above than 10)
-m, --messages   a number of issued messages per a  a single client (optional, 1000 - by default)

```

### Samples of usage:

```
PriorityQueue -c 5 
// launches 5 clients, each of them issues 1000 messages 

PriorityQueue -c 8 -m 500
// launches 8 clients, each of them issues 500 messages 

```


*Application produces a log file for each session in sub-folder "logs" of the current directory within it is started.
Supports Ctr+Break handling for aborting of its execution.*


### Used third-party code:

 - "Lightweight C++ command line option parser (https://github.com/jarro2783/cxxopts)
- "spdlog" for logging (see: https://github.com/gabime/spdlog)


### Building:
-------------------------------------------------------------------------

Project building is based on using CMake tool.

To get a "Release" build, do the simple steps:

```
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ../.
    cmake --build ./ --target all

```


Project has been build and tested on the next OS:

- Window 7 (64-bit):
  - IDE - Visual Studio 2017 with supporting of CMake-based projects feature
        (https://blogs.msdn.microsoft.com/vcblog/2016/11/16/cmake-support-in-visual-studio-the-visual-studio-2017-rc-update/)

- Linux (Ubuntu 17.10, 64-bit):
   - IDE - CLion (2017.3)
     (gcc (Ubuntu 7.2.0-8ubuntu3) 7.2.0)