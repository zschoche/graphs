/*
*
*	Author: Philipp Zschoche, https://zschoche.org
*
*/

#include <iostream>
#include <fstream>
#include "graphs.hpp"
#include "coloring.hpp"

int main(int argc, char *argv[]) {
	using namespace graphs::list;
	std::string s = "0:1,2;1:0,3;2:0,3,4;3:1,2,5;4:2,5;5:3,4;;";
	int c_max = 3;
	if (argc == 3) {
		c_max =std::atoi(argv[1]);
		s = argv[2];
	} else {
		try {
		std::stringstream ss;
		std::ifstream file("24.ag");
		std::string content((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>()); 
		auto index = content.find(' ');
		c_max = std::atoi(content.substr(0, index).c_str());
		s = content.substr(index+1);
		} catch (std::exception& e) {
			std::cout << "Could not parse file 24.ag; using hard coded graph." << std::endl;
		}
	}
	auto g = graphs::parse<graph>(s);
	//std::cout << "Graph G: " << g << std::endl;
	auto colors = graphs::coloring::backtracking_color<graph>::compute(c_max, g);
	if(colors) {
		std::cout << *colors << std::endl;
	} else {
		std::cout << "Es gibt keine Lösung." << std::endl;
	}

	return 0;
}
