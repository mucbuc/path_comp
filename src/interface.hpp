#pragma once

#include <array>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream> 

namespace path_comp {

template <typename value_type>
struct Span {
    const value_type* data;
    const size_t size;
};

template <typename scalar_type, int rank = 2, typename index_type = uint32_t>
struct Builder {
    using vector_type = std::array<scalar_type, rank>;

    class Loop {
        friend class Builder;
        void segment_end(vector_type dest);
        std::vector<vector_type> m_points;
        std::vector<index_type> m_segments;

    public:
        Loop(scalar_type* begin);
        Loop& line(scalar_type* dest);
        Loop& curve(scalar_type* control, scalar_type* dest);
        Loop& curve(scalar_type* control1, scalar_type* control2, scalar_type* dest);
        Span<scalar_type> points() const;
        Span<index_type> segments() const;
    };

    Builder();

    void append(const Loop& l);
    void append(const Loop& l, index_type);

    template <typename T, typename U, typename V>
    void traverse(T on_loop_begin, U on_segment_end, V on_loop_end) const;

    std::ostream& write_point_at(size_t index, std::ostream&) const;

    Span<scalar_type> points() const;
    Span<index_type> segments() const;
    Span<index_type> loops() const;
    Span<index_type> indecies() const;

private:
    struct PathDataPimpl;
    std::shared_ptr<PathDataPimpl> m_path_data;
};

template <typename T>
std::string make_html_path(const Builder<T, 2>& builder, std::string name);

#include "impl.hpp"
#include "make_html_path.hpp"

} // path_comp
