 # PANOCPP
Panoc algorithm in c++, the goal is to provide a single header file that contains a working high performant panoc solver.

You can find a first version of the solver at build-header/panocpplib.hpp. And an example at build-header/exampleBox.cpp

compile the examples with the panocpplib in the same directory and C++17 enabled:
msvc: cl /std:17 exampleBox.cpp 
clang++: clang++ -std=c++17 exampleBox.cpp
g++: g++ -std=c++17 exampleBox.cpp

At this stage the solver works, but does not have the optimal performance yet. The current goal is to have a production grade version by the end of 2021.
