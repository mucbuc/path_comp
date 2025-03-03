#pragma once

#include <sstream>

using namespace std;

namespace private_path_comp {

template <typename T, typename U, typename V>
static void start_loop(const T& builder, U begin, V id, ostream& result)
{
    result << "<path id=\"" << id << "\" d=\"M";
    builder.write_point_at(begin, result);
}

static void end_loop(ostream& result)
{
    result << "Z\"";
    result << "/>\n";
}

template <typename T, typename U>
static void end_segment(const T& builder, U begin, U end, ostream& result)
{
    switch (end - begin) {
    case 1:
        result << " L";
        break;
    case 2:
        result << " Q";
        break;
    case 3:
        result << " C";
        break;
    default:
        ASSERT(false);
    }

    builder.write_point_at(begin, result);
    ++begin;
    while (begin != end) {
        result << " ";
        builder.write_point_at(begin, result);
        ++begin;
    }
}

} // private_path_comp

template <typename T>
std::string make_html_path(const Builder<T, 2>& builder, std::string name)
{
    using namespace private_path_comp;
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
