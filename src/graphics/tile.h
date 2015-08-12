#ifndef TILE_H
#define TILE_H
#include "graphics/renderable.h"

namespace Graphics {
  class Tile : public Renderable {
    public:
      Tile() = delete;
      Tile(const unsigned int vertex_array_object, const VertexData& data);
      virtual ~Tile();

      virtual void destroy() override;
      virtual const bool onUpdate(const double delta) override;

      void setTexture(std::shared_ptr<BaseTexture> texture_object) noexcept;
      std::shared_ptr<BaseTexture> getTexture() const noexcept;
  };
}

#endif
