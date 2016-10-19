#include "map.h"

namespace Game {
  Map::Map(const std::string& name) : name(name) {
    map_impl = new Tmx::Map();
    map_impl->ParseFile(name);
  }

  Tmx::Map* Map::getImpl() const noexcept {
    return map_impl;
  }

  const std::string Map::getName() const noexcept {
    return name;
  }
}