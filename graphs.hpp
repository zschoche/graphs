/*
*
*	Author: Philipp Zschoche, https://zschoche.org
*
*/
#ifndef __GRAPHS_HPP__
#define __GRAPHS_HPP__

#include <memory>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <ostream>

namespace graphs {

namespace matrix {

template <typename G, typename Iter> struct neighbours {
	const G *g;
	const int v;

	neighbours(const G *_g, int _v) : g(_g), v(_v) {}

	inline Iter begin() const { return Iter(g, v); }

	inline Iter end() const { return Iter(g, v, g->vertex_count); }
};

/* Graph
 * 0 - 3
 * 4 - 2
 * 4 - 4
 * 2 - 1
Matrix:
0
0 0
0 x 0
x 0 0 0
0 x 0 0 x
*/
struct graph {

	graph(std::size_t _vertex_count)
	    : vertex_count(_vertex_count),
	      E(new bool[(vertex_count * (vertex_count + 1)) / 2]) {
		std::fill_n(E.get(), (vertex_count * (vertex_count + 1)) / 2,
			    false);
	}

	const std::size_t vertex_count;

	inline void add_edge(int v, int w) { E[get_pos(v, w)] = true; }
	inline void remove_edge(int v, int w) { E[get_pos(v, w)] = false; }

	inline bool has_edge(int v, int w) const { return E[get_pos(v, w)]; }

	class neighbours_iter
	    : public std::iterator<std::input_iterator_tag, int> {

		const graph *g;
		const int v;
		int w;

		void next() {
			do {
				w++;
			} while (!g->has_edge(v, w) && w < g->vertex_count);
		}

	      public:

		neighbours_iter(const graph *_g, int _v, int _w = 0)
		    : g(_g), v(_v), w(_w) {}
		neighbours_iter(const neighbours_iter &mit)
		    : g(mit.g), v(mit.v), w(mit.w) {}

		neighbours_iter &operator++() {
			next();
			return *this;
		}

		neighbours_iter operator++(int) {
			neighbours_iter tmp(*this);
			operator++();
			return tmp;
		}
		bool operator==(const neighbours_iter &rhs) {
			return g == rhs.g && v == rhs.v && w == rhs.w;
		}

		bool operator!=(const neighbours_iter &rhs) {
			return !(*this == rhs);
		}

		int operator*() { return w; }
	};

	inline neighbours<graph, neighbours_iter> neighbours_of(int v) const {
		return neighbours<graph, neighbours_iter>(this, v);
	}

      private:
	std::unique_ptr<bool[]> E;

	std::size_t get_pos(int v, int w) const {
		if (v < w) {
			return ((w * (w + 1)) / 2) + v;
		} else {
			return ((v * (v + 1)) / 2) + w;
		}
	}
};

} /* namespace matrix */

template <typename G> std::string to_string(const G &g) {
	std::stringstream ss;
	for (int v = 0; v < g.vertex_count; v++) {
		ss << v << ":";
		bool have_edges = false;
		for (const auto &w : g.neighbours_of(v)) {
			if (have_edges) {
				ss << ",";
			} else {
				have_edges = true;
			}
			ss << w;
		}
		ss << ";";
	}
	ss << ";";
	return ss.str();
}

namespace matrix {
inline std::ostream &operator<<(std::ostream &stream, const matrix::graph &g) {
	stream << to_string(g);
	return stream;
}
} /* namespace matrix */


namespace list {
struct graph {

	graph(std::size_t _vertex_count)
	    : vertex_count(_vertex_count),E(_vertex_count) {
	}

	const std::size_t vertex_count;

	inline void add_edge(int v, int w) {
		E[v].push_back(w);
		std::sort(E[v].begin(), E[v].end());
		//E[w].push_back(v);
		//std::sort(E[w].begin(), E[w].end());
	}
	/*
	inline void remove_edge(int v, int w) { 
		E[get_pos(v, w)] = false; 
	}
	*/

	inline bool has_edge(int v, int w) const { return std::binary_search (E[v].begin(), E[v].end(), w); }

	inline const std::vector<int>& neighbours_of(int v) const {
		return E[v];
	}

      private:
	std::vector<std::vector<int>> E;
};
	
} /* namespace list */


template <typename G>
G parse(std::string str) /* yes, copy! we need it anyway. */ {
	boost::algorithm::trim_right_if(str, boost::is_any_of(";\r\n\0"));
	std::vector<std::string> vs;
	boost::split(vs, str, boost::is_any_of(";"));
	G g(vs.size());
	for (auto &vs_str : vs) {
		std::size_t pos = vs_str.find(':');
		auto v = boost::lexical_cast<int>(vs_str.substr(0, pos));
		std::vector<std::string> es;
		auto elist = vs_str.substr(pos + 1);
		boost::split(es, elist, boost::is_any_of(","));
		for (auto &es_str : es) {
			auto w = boost::lexical_cast<int>(es_str);
			g.add_edge(v, w);
		}
	}
	return g;
}

} /* namespace graphs */

#endif /* __GRAPHS_HPP__ */
