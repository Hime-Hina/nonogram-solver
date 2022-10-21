cmake_minimum_required(VERSION 3.22)
project(nonogram)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)

set(HEADERS
    includes/Graph/Edge.h
    includes/Graph/AdjacencyList.hpp
    includes/Graph/Definitions.hpp
    includes/Graph/TopologicalSort.hpp
    includes/Graph/Scc.hpp
    includes/Graph/MaxFlow.hpp
    includes/Utilities/Array2D.hpp
    includes/Utilities/Array3D.hpp
    includes/Utilities/DisjointSet.h
    includes/Utilities/Timer.h
    )
set(SOURCES
    src/test1.cpp
    src/Graph/AdjacencyList.cpp
    src/Graph/TopologicalSort.cpp
    src/Graph/Scc.cpp
    src/Graph/MaxFlow.cpp
    src/Utilities/DisjointSet.cpp
    src/Utilities/Timer.cpp
    )

add_executable(test1 ${SOURCES} ${HEADERS})
