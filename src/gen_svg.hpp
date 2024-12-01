#pragma once

#include <iostream>
#include <sstream>

#include <cassert>
#define ASSERT(p) assert((p))

#include "interface.hpp"

using namespace std;

namespace path_comp {

std::string gen_svg(const path_comp::Builder<float, 2>& builder, std::string name);

}
