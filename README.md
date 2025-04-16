# $\color{teal}\textit{\textbf{PriorityQueue}}$

![C++](https://img.shields.io/badge/C++-17-purple?logo=C++)
![cmake](https://img.shields.io/badge/cmake-3.30-olive)
![gtest](https://img.shields.io/badge/GTest-1.14.0-blueviolet)
![Ubuntu](https://img.shields.io/badge/Ubuntu-18.04+-red?logo=Ubuntu)
![Windows](https://img.shields.io/badge/Windows-11-blue?logo=Windows)



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

To build the project under Linux OS use build.sh script with build type specification:

```
    ./build.sh release (debug)
```

On Windows:
  - Microsoft Visual Studio with supporting CMake-based projects feature and 
    compiler enables C++ 17 standard (since VS 2019)


