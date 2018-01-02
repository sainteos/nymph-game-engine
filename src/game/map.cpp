#include "map.h"

namespace Game {
  Map::Map(const std::string& path) : path(path) {
    map_impl = new Tmx::Map();
    map_impl->ParseFile(path);
  }

  Tmx::Map* Map::getImpl() const noexcept {
    return map_impl;
  }

  std::string Map::getPath() const noexcept {
    return path;
  }
}
