#ifndef MAP_H
#define MAP_H
#include <tmx/Tmx.h>

namespace Game {
  /**
   * @brief      Class wrapper for Tmx::Map
   */
  class [[scriptable]] Map {
    private:
      Tmx::Map* map_impl;
      std::string path;
    public:
      Map() = delete;
      virtual ~Map() { if(!map_impl) delete map_impl; }
      /**
       * @brief      Gets the implementation (the actual TMX map)
       *
       * @return     Returns the actual Tmx::Map contained in this wrapper.
       */
      Tmx::Map* getImpl() const noexcept;

      /**
       * @brief      Map Constructor
       *
       * @param[in]  name  The map path
       */
      [[scriptable]] Map(const std::string& path);
      /**
       * @brief      Copy constructor
       *
       * @param[in]  map   Other map wrapper class
       */
      [[scriptable]] Map(const Map& map) = default;
      /**
       * @brief      Gets the path.
       *
       * @return     The path.
       */
      [[scriptable]] std::string getPath() const noexcept;
      /**
       * @brief      equals operator for map
       *
       * @param[in]  map   The other map
       *
       * @return     returns *this
       */
      [[scriptable]] Map& operator=(const Map& map) = default;
  };
}

#endif
