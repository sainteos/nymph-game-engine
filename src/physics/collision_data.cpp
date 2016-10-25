#include "collision_data.h"

namespace Physics {

  CollisionData::CollisionData(const unsigned int map_width, const unsigned int map_height) {
    collision_table = std::vector<std::vector<int>>(map_height, std::vector<int>(map_width, -1));

  }

  void CollisionData::setIndexCollidable(const unsigned int x, const unsigned int y, const unsigned int collide_level) {
    collision_table[y][x] = collide_level;
  }

  const int CollisionData::getCollideLevel(const unsigned int x, const unsigned int y) {
    return collision_table[y][x];
  }

  void CollisionData::onStart() {

  }

  const bool CollisionData::onUpdate(const double delta) {

  }

  void CollisionData::onDestroy() {

  }

  const unsigned long long CollisionData::getValueForSorting() const {
    return 5;
  }

  const std::string CollisionData::to_string() const noexcept {
    std::stringstream str;
    str << "Collision Data: \n";
    for(auto row : collision_table) {
      for(auto element : row) {
        str << element << ", ";
      }
      str << "\n";
    }

    return str.str();
  }

  void CollisionData::log(el::base::type::ostream_t& os) const  {
    os << to_string();
  }
}