/*
*
*	Author: Philipp Zschoche, https://zschoche.org
*
*/
#ifndef __COLORING_HPP__
#define __COLORING_HPP__

#include "graphs.hpp"
#include <algorithm>
#include <boost/optional.hpp>

namespace graphs {

namespace coloring {

template <typename G> struct graph_coloring {

	graph_coloring(const G *_g, std::vector<int> _by_vertices)
	    : graph(_g), by_vertices(std::move(_by_vertices)) {}

	const G *graph;
	std::vector<int> by_vertices;

	std::string to_string() const {
		std::stringstream ss;
		ss << *std::max_element(by_vertices.begin(), by_vertices.end())
		   << "-Färbung: ";
		for (const auto &item : by_vertices) {
			ss << item << ",";
		}
		auto s = ss.str();
		return s.substr(0, s.size() - 1);
	}

	inline std::size_t count() const {}
};

template <typename G>
std::ostream &operator<<(std::ostream &stream, const graph_coloring<G> &c) {
	stream << c.to_string();
	return stream;
}

template <typename G> graph_coloring<G> greedy_color(const G &g) {
	int color = 0;
	int counter = 0;
	std::vector<int> selected_colors(g.vertex_count);
	while (counter < g.vertex_count) {
		color++;
		for (int v = 0; v < g.vertex_count; v++) {
			if (selected_colors[v] < 1 &&
			    selected_colors[v] != -color) {
				selected_colors[v] = color;
				counter++;
				for (const auto &w : g.neighbours_of(v)) {
					if (selected_colors[w] < 1) {
						selected_colors[w] = -color;
					}
				}
			}
		}
	}
	return graph_coloring<G>(&g, selected_colors);
}

template <typename G> struct backtracking_color {

	static inline boost::optional<graph_coloring<G> >
	compute(unsigned int colors, const G &g) {
		backtracking_color<G> bc(colors, g);
		if (bc.get_coloring(1)) {
			return boost::optional<graph_coloring<G> >(
			    graph_coloring<G>(&bc.graph, bc.selected_colors));
		} else {
			return boost::optional<graph_coloring<G> >();
		}
	}

      private:

	backtracking_color(unsigned int _colors, const G &_g)
	    : colors(_colors), graph(_g), selected_colors(_g.vertex_count),
	      problem(_g.vertex_count, std::vector<int>(_colors+1, -1)), going_back_to(_g.vertex_count) {
		      selected_colors[0] = 1; //wir setzen die Fabe direkt. es gibt immer eine permutation.
	}

	bool get_coloring(int v) {
		int c;
		do {
			going_back_to = -1;
			
			c = get_color(v);
			selected_colors[v] = c;
			if (c == 0) {
				return false;
			} else if (v == graph.vertex_count - 1 ||
				   get_coloring(v + 1)) {
				return true;
			}
		} while (going_back_to == -1 || going_back_to >= v);
		return false;
	}

	// bis v hat bereits alles eine Fabe. hinter v ist alles egal.
	int get_color(int v) {
		int c = selected_colors[v];
		bool invalid_color = true;
		do {
			c = (c + 1) % (colors + 1);
			selected_colors[v] = c;
			if (c == 0) { // alles probliert -> geht nicht
				break;
			} else {
				int p = problem[v][c]; // erstmal das letzte problem anschauen.
				if (p != -1 && p != v && selected_colors[p] == c) {
					if(p > going_back_to) {
						going_back_to = p;
					}
					invalid_color = true;
				} else {
					invalid_color = false;
					for (const auto &w :
					     graph.neighbours_of(v)) {
						if(w > v) { // der Rest ist egal
							break;
						} else if (selected_colors[w] == c) {
							problem[v][c] = w;
							if(w > going_back_to) {
								going_back_to = w;
							}
							invalid_color = true;
							break;
						}
					}
				}
			}
		} while (invalid_color);
		if(c != 0)
			going_back_to = graph.vertex_count;
		return c;
	}

	void print() const {
		for (const auto &item : selected_colors) {
			std::cout << item << ",";
		}
		std::cout << "("<< going_back_to << ")" << std::endl;
	}

	const unsigned int colors;
	const G &graph;
	std::vector<int> selected_colors;
	std::vector<std::vector<int> > problem;
	int going_back_to;
};

} /* namespace coloring */

} /* namespace graphs */

#endif /* __COLORING_HPP__ */
