cmake_minimum_required(VERSION 3.10)
project(guitar_graph_solo_cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
add_executable(guitar_solo
src/fretboard.cpp
src/chords.cpp
src/arpeggio.cpp
src/graph.cpp
src/dijkstra_solo.cpp
src/prim_variations.cpp
src/structure_scc.cpp
src/structure_topo.cpp
0. Configuração do projeto
Objetivo
Estrutura sugerida
CMakeLists.txt inicial
src/midi_export.cpp
src/main.cpp
)