/*
*
*	Author: Philipp Zschoche, https://zschoche.org
*
*/

#include <iostream>
#include "graphs.hpp"
#include "coloring.hpp"

int main(int argc, char *argv[]) {
	using namespace graphs::matrix;
	std::string s;
	if (argc == 2) {
		s = argv[1];
	} else {
		s = "0:1,2;1:0,3;2:0,3,4;3:1,2,5;4:2,5;5:3,4;;";
	}
	auto g = graphs::parse<graph>(s);
	std::cout << "Graph G: " << g << std::endl;
	auto colors = graphs::coloring::greedy_color(g);
	std::cout << "Greedy: " << std::endl << "\t" << colors
		  << std::endl;

	return 0;
}
