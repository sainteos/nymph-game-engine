#ifndef ANIMATED_TILE_H
#define ANIMATED_TILE_H
#include "graphics/tile.h"
#include "graphics/vertex_data.h"

namespace Graphics {
  class AnimatedTile : public Tile {
    public:
      AnimatedTile(const unsigned int vertex_array_object, const VertexData& data);
  };
}

#endif
