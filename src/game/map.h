#ifndef MAP_H
#define MAP_H
#include <tmx/Tmx.h>

//= SCRIPTABLE

namespace Game {
  class Map {
    private:
      Tmx::Map* map_impl;
      std::string name;
    public:
      Map() = delete;
      virtual ~Map() { if(!map_impl) delete map_impl; }
      Tmx::Map* getImpl() const noexcept;
      //= BEGIN SCRIPTABLE
      Map(const std::string& name);
      Map(const Map& map) = default;
      const std::string getName() const noexcept;
      Map& operator=(const Map& map) = default;
      //= END SCRIPTABLE
  };
}

#endif
