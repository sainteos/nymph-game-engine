#ifndef RENDERABLE_FACTORY_H
#define RENDERABLE_FACTORY_H
#include <memory>
#include "graphics/vertex_data.h"
namespace Graphics {
  class RenderableFactory {
    private:
      unsigned int tile_vertex_array_binding;

      const VertexData generateCube();
      const VertexData generateTile();
    public:
      RenderableFactory();
      ~RenderableFactory();
      template<class T>
      std::shared_ptr<T> create();
      template<class T>
      std::shared_ptr<T> create(const VertexData& vertex_data);
  };
}

#endif
