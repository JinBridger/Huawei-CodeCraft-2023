#pragma once
#include "workbench.hpp"

#include <unordered_map>
#include <vector>


namespace msc {
    class adjacency_list {
    public:
        using weight_type     = double;  // distance
        using value_type      = workbench;
        using size_type       = size_t;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using difference_type = std::ptrdiff_t;

    private:
        struct vertex_type {
            vertex_type(value_type&& v) : value(std::move(v)) {}
            vertex_type(const_reference v) : value(v) {}

            value_type value;
        };

    public:
        void add_edge(const_reference vertex_a, const_reference vertex_b, weight_type w = {}) {
            vertex_type vtx_a(vertex_a), vtx_b(vertex_b);
            add_directed_edge(vtx_a, vtx_b, w);
            add_directed_edge(vtx_b, vtx_a, w);
        }

        bool add_vectex(const_reference new_vertex) {
        auto [_, success] = index_map.try_emplace(new_vertex, vertices.size());
        if (success)
            vertices.emplace_back(std::move(vertex_type(new_vertex)), std::vector<edge>{});
        return success;
    }

    private:
        struct edge {
            edge(const vertex_type& _vertex, const weight_type weight) : vertex(_vertex), weight(weight) {}
            vertex_type vertex;
            weight_type weight;
        };

        struct header {
            vertex_type       vertex;
            std::vector<edge> edges;
        };

        void add_directed_edge(const vertex_type& vertex_a, const vertex_type& vertex_b, weight_type w = {}) {
            // if (vertex_a == vertex_b)
            //     return false;
            add_vectex(vertex_a.value);
            add_vectex(vertex_b.value);
            auto& edges = vertices[index_map[vertex_a.value]].edges;

            edges.emplace_back(vertex_b, w);
        }

        std::vector<header> vertices;

        std::unordered_map<value_type, size_type> index_map;
    }
}  // namespace msc