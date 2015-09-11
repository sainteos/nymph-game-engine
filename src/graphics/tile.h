#ifndef TILE_H
#define TILE_H
#include "graphics/renderable.h"

namespace Graphics {
  class Tile : public Renderable {
    private:
      unsigned int size_pixels;
    public:
      Tile() = delete;
      Tile(const unsigned int vertex_array_object, const VertexData& data);
      virtual ~Tile();

      virtual void onDestroy() override;
      virtual const bool onUpdate(const double delta) override;

      void setSizeInPixels(const unsigned int size);
      const unsigned int sizeInPixels() const noexcept;
  };
}

#endif
