#pragma once

#include <atomic>
#include <sstream>
#include <string>

#include <CTML/include/ctml.hpp>
#include <path_comp/interface.hpp>

struct Rect {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

template <typename scalar_type, int rank>
std::string generate_svg(const path_comp::Builder<scalar_type, rank>& builder, Rect canvas)
{
    using namespace CTML;

    Node group("group");

    size_t segement_begin(0);
    const auto& segments = builder.segments();

    size_t loop_begin(0);
    for (const auto l& : builder.loops()) {
        stringstream path;
        for (auto i = loop_begin; i < l; ++i) {
        }

        loop_begin = l;
        /*
        Node path("path");


        // get ponts

        group.AppendChild(path);
*/
    }

    return "";
}
