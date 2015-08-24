#ifndef RENDERABLE_FACTORY_H
#define RENDERABLE_FACTORY_H
#include <memory>
#include <tmx/Tmx.h>
#include "graphics/vertex_data.h"
#include "graphics/texture_manager.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
namespace Graphics {
  class RenderableFactory {
    private:
      unsigned int animated_tile_vao;
      const VertexData generateCube();
      const VertexData generateTile();
      unsigned int generateVertexArrayObject(VertexData vertex_data);
    public:
      RenderableFactory();
      ~RenderableFactory();
      template<class T>
      std::shared_ptr<T> create();
      template<class T>
      std::shared_ptr<T> create(const VertexData& vertex_data);

      std::vector<std::shared_ptr<Renderable>> createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const ShaderManager& shader_manager);
  };
}

#endif
