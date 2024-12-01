#include <cassert>
#define ASSERT(p) assert((p));

#include "gen_svg.hpp"

#include <iostream>
#include <sstream>

using namespace std;

namespace {

enum { rank = 2 };

using builder_type = path_comp::Builder<float, rank>;
using Loop = builder_type::Loop;
using vt = std::array<float, rank>;

template <typename T, typename U>
static void start_loop(const builder_type& builder, T begin, U id, ostream& result)
{
    result << "<path id=\"" << id << "\" d=\"M";
    builder.write_vector(begin, result);
}

static void end_loop(ostream& result)
{
    result << "Z\"";
    result << "/>\n";
}

template <typename T>
static void end_segment(const builder_type& builder, T begin, T end, ostream& result)
{
    switch (end - begin) {
    case 1:
        result << "L";
        break;
    case 2:
        result << "C";
        break;
    case 3:
        result << "Q";
        break;
    default:
        ASSERT(false);
    }

    builder.write_vector(begin, result);
    ++begin;
    while (begin != end) {
        result << ", ";
        builder.write_vector(begin, result);
        ++begin;
    }
}

}

namespace path_comp {

std::string gen_svg(const path_comp::Builder<float, 2>& builder, std::string name)
{
    std::stringstream result;
    builder.traverse(
        [&result, &builder, name](auto point_index, auto id) {
            start_loop(builder, point_index, name + std::to_string(id), result);
        },
        [&result, &builder](auto segment_begin, auto segment_end) {
            end_segment(builder, segment_begin, segment_end, result);
        },
        [&result, &builder](auto point_index) {
            end_loop(result);
        });

    return result.str();
}

}
