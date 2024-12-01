#pragma once

#include "interface.hpp"
#include <string>

using namespace std;
namespace path_comp {

std::string gen_svg(const path_comp::Builder<float, 2>& builder, std::string name);

}
