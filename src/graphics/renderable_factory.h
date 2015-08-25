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
      struct RenderableInfo {
        VertexData vertex_data;
        unsigned int vertex_array_object;
        unsigned int gid;
        bool flip_horizontal;
        bool flip_vertical;
        bool flip_diagonal;
      };

      unsigned int animated_tile_vao;
      const VertexData generateCube();
      const VertexData generateTile();
      unsigned int generateVertexArrayObject(VertexData vertex_data);
      std::shared_ptr<BaseTexture> textureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::vector<glm::vec2> generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height);
    public:
      RenderableFactory();
      ~RenderableFactory();
      template<class T>
      std::shared_ptr<T> create();
      template<class T>
      std::shared_ptr<T> create(const VertexData& vertex_data);

      std::vector<std::shared_ptr<Renderable>> createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
  };
}

#endif
