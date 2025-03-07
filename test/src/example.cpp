#include <iostream>
#include <sstream>

#include <asserter/src/test.hpp>
#include <path_comp/src/interface.hpp>

using namespace std;

using loop_type = path_comp::Loop<std::array<float, 2>>;

static void test_points_and_segments()
{
    auto b = loop_type({ 30, 50 })
                 .line({ 10, 60 })
                 .curve({ 50, 20 }, { 100, 100 })
                 .curve({ 50, 20 }, { 100, 100 }, { 100, 100 })
                 .line({ 10, 30 });

    ASSERT(b.points().size() == 8);
    ASSERT(b.segments().size() == 4);

    auto c = loop_type({ 1, 1 });

    ASSERT(c.points().size() == 1);
    ASSERT(c.segments().size() == 0);

    FOOTER;
}

int main()
{
    test_points_and_segments();
    return 0;
}
