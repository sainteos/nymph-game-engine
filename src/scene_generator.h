#ifndef SCENE_GENERATOR_H
#define SCENE_GENERATOR_H
#include <memory>
#include <map>
#include <tmx/Tmx.h>
#include "graphics/vertex_data.h"
#include "graphics/texture_manager.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
#include "graphics/tile_animator.h"
#include "component_manager.h"
#include "entity.h"
#include "scene.h"

class SceneGenerator {
  private:

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
      std::shared_ptr<Graphics::TileAnimator> animator;
    };


    std::shared_ptr<ComponentManager> component_manager;
    std::weak_ptr<Graphics::TextureManager> texture_manager;
    std::weak_ptr<Graphics::ShaderManager> shader_manager;
    std::map<std::string, DynamicAnimation> dynamic_animations;

    const Graphics::VertexData generateBasisCube();
    const Graphics::VertexData generateBasisTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0, const float z_order = -1.0, const unsigned int offset_x = 0, const unsigned int offset_y = 0);
    std::shared_ptr<Graphics::BaseTexture> textureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
    std::shared_ptr<Graphics::BaseTexture> normalTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
    std::shared_ptr<Graphics::BaseTexture> displacementTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
    std::vector<glm::vec2> generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height);
    std::vector<glm::vec3> generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0, const float z_order = -1.0, const unsigned int offset_x = 0, const unsigned int offset_y = 0);


    MapRenderables createRenderablesFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Tmx::Map& map);
    std::vector<std::shared_ptr<Entity>> createStaticallyAnimatedTilesFromMap(const Tmx::Map& map);
    std::map<std::string, DynamicAnimation> createAnimationsFromAnimationMap(const Tmx::Map& map);
    std::vector<std::shared_ptr<Component>> createLightsFromMap(const Tmx::Map& map);
  public:
    SceneGenerator(const Tmx::Map& animation_index, std::shared_ptr<ComponentManager> component_manager, std::shared_ptr<Graphics::TextureManager> texture_manager, std::shared_ptr<Graphics::ShaderManager> shader_manager);
    SceneGenerator() = delete;
    ~SceneGenerator();
    
    std::shared_ptr<Scene> createSceneFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Tmx::Map& map);

    std::shared_ptr<Entity> getDynamicEntityByName(const std::string& name);
};

#endif
