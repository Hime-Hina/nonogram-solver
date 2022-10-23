cmake_minimum_required(VERSION 3.22)
project(nonogram)

set(CMAKE_CXX_STANDARD 17)

include_directories(includes)

set(HEADERS
    includes/Graph/Edge.hpp
    includes/Graph/AdjacencyList.hpp
    includes/Definitions.hpp
    includes/Graph/TopologicalSort.hpp
    includes/Graph/Scc.hpp
    includes/Graph/MaxFlow.hpp
    includes/Utilities/Array2D.hpp
    includes/Utilities/Array3D.hpp
    includes/Utilities/DisjointSet.h
    includes/Utilities/Timer.h
    includes/Solver.hpp
    includes/Utilities/UtilityTemplates.hpp
    )

set(SOURCES
    src/Graph/AdjacencyList.cpp
    src/Graph/TopologicalSort.cpp
    src/Graph/Scc.cpp
    src/Graph/MaxFlow.cpp
    src/Utilities/DisjointSet.cpp
    src/Utilities/Timer.cpp
    src/Solver.cpp
    )

add_executable(Test src/test/Test.cpp)
add_executable(TestMaxFlow src/test/TestMaxFlow.cpp ${SOURCES} ${HEADERS})
add_executable(TestArray2D src/test/TestArray2D.cpp ${SOURCES} ${HEADERS})
add_executable(TestArray3D src/test/TestArray3D.cpp ${SOURCES} ${HEADERS})
add_executable(solver src/main.cpp ${SOURCES} ${HEADERS})
