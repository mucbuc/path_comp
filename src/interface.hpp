#pragma once

#include <vector>

namespace path_comp {

template <typename Vector_t, typename Index_t = uint32_t>
struct Loop {
    using scalar_type = typename Vector_t::value_type;
    using vector_type = Vector_t;
    using index_type = Index_t;

    Loop(vector_type begin);
    Loop& line(vector_type dest);
    Loop& curve(vector_type control, vector_type dest);
    Loop& curve(vector_type control1, vector_type control2, vector_type dest);

    std::vector<vector_type> points() const;
    std::vector<index_type> segments() const;

private:
    void segment_end(vector_type dest);
    std::vector<vector_type> m_points;
    std::vector<index_type> m_segments;
};

#include "impl.hpp"

} // path_comp
