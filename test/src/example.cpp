#include <iostream>
#include <sstream>

// #include <cassert>
// #define ASSERT(p) assert((p))

#include <path_comp/src/gen_svg.hpp>
#include <path_comp/src/interface.hpp>

using namespace std;

using builder_type = path_comp::Builder<float, 2>;
using Loop = builder_type::Loop;
using vt = std::array<float, 2>;

void test_loop()
{
    builder_type b;

    b.append(Loop(vt { { 11, 12 } }.data())
                 .line(vt { { 13, 14 } }.data())
                 .line(vt { { 15, 16 } }.data())
                 .curve(vt { { 17, 18 } }.data(), vt { { 19, 20 } }.data())
                 .curve(vt { { 21, 22 } }.data(), vt { { 23, 24 } }.data(), vt { { 25, 26 } }.data()));

    b.append(Loop(vt { { 1, 2 } }.data())
                 .line(vt { { 3, 4 } }.data())
                 .curve(vt { { 5, 6 } }.data(), vt { { 7, 8 } }.data(), vt { { 9, 10 } }.data())
                 .line(vt { { 11, 12 } }.data()),
        21);

    cout << gen_svg(b, "triangle_") << std::endl;
}

static void test_svg()
{
    builder_type b;

    b.append(Loop(vt { { 30, 50 } }.data())
                 .line(vt { { 10, 60 } }.data())
                 .line(vt { { 10, 30 } }.data()));

    cout << gen_svg(b, "triangle_") << std::endl;
}

static void test_new_API()
{
    builder_type b;

    b.append(Loop(vt { { 30, 50 } }.data())
                 .line(vt { { 10, 60 } }.data())
                 .curve(vt { { 50, 20 } }.data(), vt { { 100, 100 } }.data())
                 .curve(vt { { 50, 20 } }.data(), vt { { 100, 100 } }.data(), vt { { 100, 100 } }.data())
                 .line(vt { { 10, 30 } }.data()));

    b.append(Loop(vt { { 30, 50 } }.data())
                 .line(vt { { 10, 30 } }.data())
                 .line(vt { { 10, 30 } }.data()));

    b.traverse(
        [](auto point_index, auto index) {
            std::cout << "loop_begin " << index << ": " << point_index << std::endl;
        },
        [](auto segment_begin, auto segment_end) {
            std::cout << "segment_end " << segment_begin << " " << segment_end << std::endl;
        },
        [](auto point_index) {
            std::cout << "loop_end " << point_index << std::endl;
        });
}

int main()
{
    test_new_API();
    test_loop();
    test_svg();
    return 0;
}
