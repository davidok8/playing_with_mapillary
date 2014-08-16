cmake_minimum_required(VERSION 2.6)

add_library(graph_cut_optimization
	block.h
	LinkedBlockList.h LinkedBlockList.cpp
	graph.h graph.cpp
	energy.h
	GCoptimization.h GCoptimization.cpp
	maxflow.cpp)
	
add_executable(graph_cut_optimization_test example.cpp)
target_link_libraries(graph_cut_optimization_test graph_cut_optimization)
