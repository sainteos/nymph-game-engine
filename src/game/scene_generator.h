#ifndef SCENE_GENERATOR_H
#define SCENE_GENERATOR_H
#include <memory>
#include <map>
#include "physics/collision_data.h"
#include "graphics/vertex_data.h"
#include "graphics/texture_manager.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
#include "graphics/tile_animator.h"
#include "component_manager.h"
#include "entity.h"
#include "scene.h"
#include "map.h"
//= SCRIPTABLE

namespace Game {
  class SceneGenerator {
    private:

      struct AnimationPlaceholder {
        std::string sprite_name;
        std::string default_animation;
        int x_pos;
        int y_pos;
        float z_order;
      };

      struct MapRenderables {
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<AnimationPlaceholder> dynamic_animations;
      };

      struct DynamicAnimation {
        std::shared_ptr<Entity> entity;
        std::shared_ptr<Graphics::TileAnimator> animator;
      };

      std::weak_ptr<Graphics::TextureManager> texture_manager;
      std::weak_ptr<Graphics::ShaderManager> shader_manager;
      std::map<std::string, DynamicAnimation> dynamic_animations;

      const Graphics::VertexData generateBasisCube();
      const Graphics::VertexData generateBasisTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0,  const unsigned int offset_x = 0, const unsigned int offset_y = 0);
      std::shared_ptr<Graphics::BaseTexture> textureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
      std::shared_ptr<Graphics::BaseTexture> normalTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
      std::shared_ptr<Graphics::BaseTexture> displacementTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path);
      std::vector<glm::vec2> generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height);
      std::vector<glm::vec3> generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos = 0, const unsigned int y_pos = 0, const unsigned int offset_x = 0, const unsigned int offset_y = 0);


      MapRenderables createRenderablesFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Map& map);
      std::vector<std::shared_ptr<Entity>> createStaticallyAnimatedTilesFromMap(const Map& map);
      std::map<std::string, DynamicAnimation> createAnimationsFromAnimationMap(const Map& map);
      std::vector<std::shared_ptr<Component>> createLightsFromMap(const Map& map);
      std::shared_ptr<Physics::CollisionData> createCollisionDataFromMap(const Map& map);

      const float calculateZ(unsigned int layer_index, unsigned int layers_total);
    public:
      SceneGenerator() = delete;
      ~SceneGenerator();

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      SceneGenerator constructor
       *
       * @param[in]  animation_index  The animation index containing all animations
       * @param[in]  texture_manager  The texture manager
       * @param[in]  shader_manager   The shader manager
       */
      SceneGenerator(const Map& animation_index, std::shared_ptr<Graphics::TextureManager> texture_manager, std::shared_ptr<Graphics::ShaderManager> shader_manager);
      /**
       * @brief      SceneGenerator copy constructor
       *
       * @param[in]  s  The other SceneGenerator
       */
      SceneGenerator(const SceneGenerator& s) = default;
      /**
       * @brief      SceneGenerator equal operator
       *
       * @param[in]  other  The other SceneGenerator
       *
       * @return     *this
       */
      SceneGenerator& operator=(const SceneGenerator& other) = default;
      
      /**
       * @brief      Creates a scene from a map.
       *
       * @param[in]  patch_width_tiles   The patch width in tiles
       * @param[in]  patch_height_tiles  The patch height in tiles
       * @param[in]  map                 The map
       *
       * @return     A newly created scene
       */
      std::shared_ptr<Scene> createSceneFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Map& map);

      /**
       * @brief      Gets a dynamic entity by name.
       *
       * @param[in]  name  The name
       *
       * @return     The dynamic entity by name.
       */
      std::shared_ptr<Entity> getDynamicEntityByName(const std::string& name);

      /**
       * @brief      Gets all dynamic entity names
       *
       * @return     The dynamic entity names.
       */
      std::vector<std::string> getDynamicEntityNames() const noexcept;

      /**
       * @brief      Strips path specification from map name
       *
       * @param[in]  path  The path
       *
       * @return     The stripped map name.
       */
      const std::string getStrippedMapName(const std::string path);
      //= END SCRIPTABLE
  };
}

#endif
