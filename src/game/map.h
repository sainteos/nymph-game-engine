#ifndef MAP_H
#define MAP_H
#include <tmx/Tmx.h>

//= SCRIPTABLE

namespace Game {
  /**
   * @brief      Class wrapper for Tmx::Map
   */
  class Map {
    private:
      Tmx::Map* map_impl;
      std::string name;
    public:
      Map() = delete;
      virtual ~Map() { if(!map_impl) delete map_impl; }
      /**
       * @brief      Gets the implementation (the actual TMX map)
       *
       * @return     Returns the actual Tmx::Map contained in this wrapper.
       */
      Tmx::Map* getImpl() const noexcept;
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Map Constructor
       *
       * @param[in]  name  The map name
       */
      Map(const std::string& name);
      /**
       * @brief      Copy constructor
       *
       * @param[in]  map   Other map wrapper class
       */
      Map(const Map& map) = default;
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      const std::string getName() const noexcept;
      /**
       * @brief      equals operator for map
       *
       * @param[in]  map   The other map
       *
       * @return     returns *this
       */
      Map& operator=(const Map& map) = default;
      //= END SCRIPTABLE
  };
}

#endif
