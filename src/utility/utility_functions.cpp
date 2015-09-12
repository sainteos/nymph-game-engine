#include <cstdlib>
#include "utility_functions.h"

namespace Utility {
  const glm::vec3 stringToVec3(const std::string str) {
    auto first_comma = str.find_first_of(",");
    auto second_comma = str.find_last_of(",");
    return glm::vec3(atof(str.substr(0, first_comma).c_str()),
                     atof(str.substr(first_comma + 1, second_comma - first_comma).c_str()),
                     atof(str.substr(second_comma + 1, str.size() - second_comma).c_str()));
  }
}
