#ifndef MAP_HELPER_H
#define MAP_HELPER_H
#include <memory>
#include <tmx/Tmx.h>
#include "graphics/vertex_data.h"
#include "graphics/texture_manager.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
#include "graphics/tile_animator.h"
#include "graphics/ui/text.h"
#include "component_manager.h"
#include "entity.h"

namespace Graphics {
  struct AnimationPlaceholder {
    std::string sprite_name;
    std::string default_animation;
    int x_pos;
    int y_pos;
    int z_order;
  };

  struct MapRenderables {
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<AnimationPlaceholder> dynamic_animations;
  };

  struct DynamicAnimation {
    std::shared_ptr<Entity> entity;
    std::shared_ptr<TileAnimator> animator;
    std::string sprite_name;
  };

  class MapHelper {
    private:
      struct RenderableInfo {
        VertexData vertex_data;
        unsigned int vertex_array_object;
        unsigned int gid;
        bool flip_horizontal;
        bool flip_vertical;
        bool flip_diagonal;
      };

      std::shared_ptr<ComponentManager> component_manager;

      unsigned int animated_tile_vao;
      const VertexData generateBasisCube();
      const VertexData generateBasisTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0, const float z_order = -1.0, const unsigned int offset_x = 0, const unsigned int offset_y = 0);
      std::shared_ptr<BaseTexture> textureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::shared_ptr<BaseTexture> normalTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::shared_ptr<BaseTexture> displacementTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name);
      std::vector<glm::vec2> generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height);
      std::vector<glm::vec3> generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0, const float z_order = -1.0, const unsigned int offset_x = 0, const unsigned int offset_y = 0);
    public:
      MapHelper(std::shared_ptr<ComponentManager> manager);
      MapHelper() = delete;
      ~MapHelper();

      MapRenderables createRenderablesFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
      std::vector<std::shared_ptr<Entity>> createStaticallyAnimatedTilesFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
      std::vector<std::shared_ptr<DynamicAnimation>> createAnimationsFromAnimationMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager);
      std::vector<std::shared_ptr<Light>> createLightsFromMap(const Tmx::Map& map);
      std::shared_ptr<UI::Text> createText(std::shared_ptr<UI::Font> font, const glm::vec4& color = glm::vec4(1.0));

  };
}

#endif
