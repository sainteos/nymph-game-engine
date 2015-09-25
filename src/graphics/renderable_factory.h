#ifndef RENDERABLE_FACTORY_H
#define RENDERABLE_FACTORY_H
#include <memory>
#include <tmx/Tmx.h>
#include "graphics/vertex_data.h"
#include "graphics/texture_manager.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
#include "graphics/tile.h"
#include "graphics/animated_tile.h"
#include "graphics/text.h"

namespace Graphics {
  struct AnimationPlaceholder {
    std::string sprite_name;
    std::string default_animation;
    int x_pos;
    int y_pos;
    int z_order;
  };

  struct MapRenderables {
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<AnimationPlaceholder> dynamic_animations;
  };

  struct Animation {
    std::shared_ptr<AnimatedTile> tile;
    std::string sprite_name;
    std::string animation_name;
  };

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
      const VertexData generateTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int offset_x = 0, const unsigned int offset_y = 0);
      unsigned int generateVertexArrayObject(VertexData vertex_data);
      std::shared_ptr<BaseTexture> textureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::shared_ptr<BaseTexture> normalTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::shared_ptr<BaseTexture> displacementTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::vector<glm::vec2> generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height);
      std::vector<glm::vec3> generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int offset_x = 0, const unsigned int offset_y = 0);
    public:
      RenderableFactory();
      ~RenderableFactory();
      template<class T>
      std::shared_ptr<T> create();
      template<class T>
      std::shared_ptr<T> create(const VertexData& vertex_data);

      std::vector<std::shared_ptr<Light>> createLightsFromMap(const Tmx::Map& map);
      MapRenderables createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
      std::vector<std::shared_ptr<AnimatedTile>> createStaticallyAnimatedTilesFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
      std::vector<Animation> createAnimationsFromAnimationMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);

      std::shared_ptr<Text> createText(std::shared_ptr<Font> font, const glm::vec4& color = glm::vec4(1.0));

  };
}

#endif
