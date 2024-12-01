#pragma once

#include "interface.hpp"
#include <string>

using namespace std;
namespace path_comp {

namespace private_path_comp
{

template <typename T, typename U>
static void start_loop(const path_comp::Builder<T, 2>& builder, size_t begin, U id, ostream& result);

static void end_loop(ostream& result);

template <typename T>
static void end_segment(const path_comp::Builder<T, 2>& builder, size_t begin, size_t end, ostream& result);

} // private_path_comp

template<typename T>
std::string gen_svg(const path_comp::Builder<T, 2>& builder, std::string name)
{
    std::stringstream result;
    builder.traverse(
        [&result, &builder, name](auto point_index, auto id) {
            private_path_comp::start_loop(builder, point_index, name + std::to_string(id), result);
        },
        [&result, &builder](auto segment_begin, auto segment_end) {
            private_path_comp::end_segment(builder, segment_begin, segment_end, result);
        },
        [&result, &builder](auto point_index) {
            private_path_comp::end_loop(result);
        });

    return result.str();
}

namespace private_path_comp
{

template <typename T, typename U>
void start_loop(const path_comp::Builder<T, 2>& builder, size_t begin, U id, ostream& result)
{
    result << "<path id=\"" << id << "\" d=\"M";
    builder.write_point_at(begin, result);
}

void end_loop(ostream& result)
{
    result << "Z\"";
    result << "/>\n";
}

template <typename T>
void end_segment(const path_comp::Builder<T, 2>& builder, size_t begin, size_t end, ostream& result)
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

    builder.write_point_at(begin, result);
    ++begin;
    while (begin != end) {
        result << ", ";
        builder.write_point_at(begin, result);
        ++begin;
    }
}

} // private_path_comp


}
