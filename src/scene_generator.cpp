#include <easylogging++.h>
#include <glm/ext.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include <algorithm>
#include <sstream>
#include <functional>
#include "scene_generator.h"
#include "graphics/renderable.h"
#include "transform.h"
#include "exceptions/invalid_filename_exception.h"
#include "graphics/light.h"
#include "utility/utility_functions.h"

SceneGenerator::SceneGenerator(const Tmx::Map& animation_index, std::shared_ptr<Graphics::TextureManager> texture_manager, std::shared_ptr<Graphics::ShaderManager> shader_manager) : texture_manager(texture_manager), shader_manager(shader_manager) {
  dynamic_animations = createAnimationsFromAnimationMap(animation_index);
}

SceneGenerator::~SceneGenerator() {
}

const Graphics::VertexData SceneGenerator::generateBasisCube() {
  std::vector<glm::vec3> verts {
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 1.0, 0.0),
    glm::vec3(1.0, 1.0, 0.0),
    glm::vec3(1.0, 0.0, 0.0),

    glm::vec3(0.0, 0.0, -1.0),
    glm::vec3(0.0, 1.0, -1.0),
    glm::vec3(1.0, 1.0, -1.0),
    glm::vec3(1.0, 0.0, -1.0),
  };

  std::vector<unsigned int> indices {
    0, 1, 2, 3, 
    0, 1, 4, 5,
    1, 5, 6, 2,

    7, 6, 5, 4,
    3, 2, 6, 7, 
    3, 7, 4, 0
  };

  Graphics::VertexData vertex_data(GL_QUADS);
  vertex_data.addIndices(indices);
  vertex_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, verts);

  return vertex_data;
}

const Graphics::VertexData SceneGenerator::generateBasisTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos, const unsigned int y_pos, const float z_order, const unsigned int offset_x, const unsigned int offset_y) {
  std::vector<glm::vec2> texs {
    glm::vec2(0.0, 0.0),
    glm::vec2(0.0, 1.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(0.0, 0.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(1.0, 0.0)
  };

  Graphics::VertexData vert_data(GL_TRIANGLES);
  vert_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, generateVertexCoords(base_width, base_height, current_width, current_height, x_pos, y_pos, z_order, offset_x, offset_y));
  vert_data.addVec<glm::vec2>(Graphics::VertexData::DATA_TYPE::TEX_COORDS, texs);
  vert_data.addVec<unsigned int>(Graphics::VertexData::DATA_TYPE::TEXTURE_UNIT, std::vector<unsigned int> { 0, 0, 0, 0, 0, 0 });

  return vert_data;
}

std::shared_ptr<Graphics::BaseTexture> SceneGenerator::textureFromTileset(const Tmx::Tileset* tileset, const std::string& path) {
  //Get out of the map directory
  auto pos = path.find_last_of("/");
  auto new_path = path.substr(0, pos);
  pos = new_path.find_last_of("/");
  new_path = new_path.substr(0, pos);

  auto tileset_image = tileset->GetImage();

  auto source = tileset_image->GetSource();
  //add path to the image source
  source.erase(0, 2);
  source = new_path + source;

  auto texture_name = Graphics::TextureManager::getNameFromPath(source);

  if(!texture_manager.lock()->textureExists(texture_name)) {
    if(!texture_manager.lock()->loadTexture(source)) {
      throw Exceptions::InvalidFilenameException(source);
    }
  }

  return (*texture_manager.lock())[texture_name];
}

std::shared_ptr<Graphics::BaseTexture> SceneGenerator::normalTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path) {
  //Get out of the map directory
  auto pos = path.find_last_of("/");
  auto new_path = path.substr(0, pos);
  pos = new_path.find_last_of("/");
  new_path = new_path.substr(0, pos);

  auto tileset_image = tileset->GetImage();

  auto source = tileset_image->GetSource();
  //add path to the image source
  source.erase(0, 2);
  //add "normal" to source name
  //find the dot for extension
  //we want normal to go before it
  auto extension_pos = source.find_last_of(".");
  source.insert(extension_pos, "-Normal");
  source = new_path + source;

  auto texture_name = Graphics::TextureManager::getNameFromPath(source);

  if(!texture_manager.lock()->textureExists(texture_name)) {
    if(!texture_manager.lock()->loadTexture(source)) {
      throw Exceptions::InvalidFilenameException(source);
    }
  }

  return (*texture_manager.lock())[texture_name];
}

std::shared_ptr<Graphics::BaseTexture> SceneGenerator::displacementTextureFromTileset(const Tmx::Tileset* tileset, const std::string& path) {
  //Get out of the map directory
  auto pos = path.find_last_of("/");
  auto new_path = path.substr(0, pos);
  pos = new_path.find_last_of("/");
  new_path = new_path.substr(0, pos);

  auto tileset_image = tileset->GetImage();

  auto source = tileset_image->GetSource();
  //add path to the image source
  source.erase(0, 2);
  //add "normal" to source name
  //find the dot for extension
  //we want normal to go before it
  auto extension_pos = source.find_last_of(".");
  source.insert(extension_pos, "-Displacement");
  source = new_path + source;

  auto texture_name = Graphics::TextureManager::getNameFromPath(source);

  if(!texture_manager.lock()->textureExists(texture_name)) {
    if(!texture_manager.lock()->loadTexture(source)) {
      throw Exceptions::InvalidFilenameException(source);
    }
  }

  return (*texture_manager.lock())[texture_name];
}

std::vector<glm::vec2> SceneGenerator::generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height) {
  std::vector<glm::vec2> texs {
    glm::vec2(-0.5, -0.5),
    glm::vec2(-0.5, 0.5),
    glm::vec2(0.5, 0.5),
    glm::vec2(-0.5, -0.5),
    glm::vec2(0.5, 0.5),
    glm::vec2(0.5, -0.5)
  };
  
  //Tile flipping
  if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Reflect across x=y
      texs[i] = texs[i] * glm::mat2(0.0, 1.0, 1.0, 0.0);
    }
  }
  else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Rotate 90 Anticlockwise
      texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
    }               
  }
  else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Rotate 90 Clockwise
      texs[i] = texs[i] * glm::mat2(0.0, 1.0, -1.0, 0.0);
    }       
  }
  else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Rotate 180
      texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, -1.0);
    }       
  }
  else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //rotate anticlockwise 90, reflect across x
      texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
      texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
    }       
  }
  else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Reflect across x
      texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
    }       
  }
  else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
    for(int i = 0; i < texs.size(); i++) {
      //Reflect across y
      texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, 1.0);
    }       
  }
  
  //Translate to bottom left at 0,0
  for(int i = 0; i < texs.size(); i++) {
    texs[i] = texs[i] + glm::vec2(0.5, 0.5);
  }

  float width = (float)tile_width / (float)texture_width;
  float height = (float)tile_height / (float)texture_height;
   
  //scale by width/height
  for(int i = 0; i < texs.size(); i++) {
    texs[i] = texs[i] * glm::vec2(width, height);
  }

  auto id = layer->GetTileId(x_pos, y_pos);

  int width_in_tiles = texture_width / tile_width;
  int height_in_tiles = texture_height / tile_height;
  int tileset_x_pos = id % width_in_tiles;
  int tileset_y_pos = height_in_tiles - 1 - id / width_in_tiles;

  //translate by pos
  for(int i = 0; i < texs.size(); i++)  {
    texs[i] += glm::vec2(tileset_x_pos * width, tileset_y_pos * height);
  }

  return texs;
}

std::vector<glm::vec3> SceneGenerator::generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int x_pos, const unsigned int y_pos, const float z_order, const unsigned int offset_x, const unsigned int offset_y) {
  std::vector<glm::vec3> verts {
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height, z_order),
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height + (float)current_height / (float)base_height, z_order),
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width + (float)current_width / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height + (float)current_height / (float)base_height, z_order),
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height, z_order),
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width + (float)current_width / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height + (float)current_height / (float)base_height, z_order),
    glm::vec3((float)x_pos + (float)offset_x / (float)base_width + (float)current_width / (float)base_width, (float)y_pos + (float)offset_y / (float)base_height, z_order)
  };
  return verts;
}

std::shared_ptr<Scene> SceneGenerator::createSceneFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Tmx::Map& map) {
  auto scene = std::make_shared<Scene>();

  auto map_renderables = createRenderablesFromMap(patch_width_tiles, patch_height_tiles, map);

  scene->addEntities(map_renderables.entities);
  
  for(auto placeholder : map_renderables.dynamic_animations) {
    auto new_entity = dynamic_animations[placeholder.sprite_name].entity;
    new_entity->getTransform()->translate(glm::vec3((float)placeholder.x_pos, (float)placeholder.y_pos, (float)placeholder.z_order));
    auto animator = dynamic_animations[placeholder.sprite_name].animator;
    animator->setStartingState(placeholder.default_animation);
    new_entity->addComponent(animator);

    scene->addEntity(new_entity);
  }
  
  auto static_animations = createStaticallyAnimatedTilesFromMap(map);

  scene->addEntities(static_animations);

  auto lights = createLightsFromMap(map);

  scene->addComponents(lights);

  scene->getTransform()->translate(glm::vec2(-map.GetWidth() / 2.0, -map.GetHeight() / 2.0));

  return scene;
}

std::shared_ptr<Entity> SceneGenerator::getDynamicEntityByName(const std::string& name) {
  if(dynamic_animations.count(name) > 0) {
    return dynamic_animations[name].entity;
  }
  else {
    return nullptr;
  }
}

std::vector<std::string> SceneGenerator::getDynamicEntityNames() const noexcept {
  std::vector<std::string> names;
  for(auto dynamic_entity : dynamic_animations) 
    names.push_back(dynamic_entity.first);
  return names;
}

std::vector<std::shared_ptr<Component>> SceneGenerator::createLightsFromMap(const Tmx::Map& map) {
  std::vector<std::shared_ptr<Component>> lights;
  std::vector<Tmx::Object*> light_map_objects;
  for(auto group : map.GetObjectGroups()) {
    std::copy_if(group->GetObjects().begin(), group->GetObjects().end(), std::back_inserter(light_map_objects),
      [&](Tmx::Object* o) {
        return o->GetProperties().HasProperty("Type") && o->GetProperties().GetStringProperty("Type") == "Light";
      }
    );
  }

  for(auto map_light : light_map_objects) {
    Graphics::Light::Type light_type = Graphics::Light::Type::POINT;
    if(map_light->GetProperties().HasProperty("LightType")) {
      light_type = Graphics::Light::stringToType(map_light->GetProperties().GetStringProperty("LightType"));
    }
    
    std::shared_ptr<Graphics::Light> new_light = std::make_shared<Graphics::Light>(light_type);
    if(map_light->GetProperties().HasProperty("Color")) {
      new_light->setColor(Utility::stringToVec3(map_light->GetProperties().GetStringProperty("Color")) / glm::vec3(256.0, 256.0, 256.0)); 
    }
    if(map_light->GetProperties().HasProperty("Intensity")) {
      new_light->setIntensity(map_light->GetProperties().GetFloatProperty("Intensity"));
    }
    if(map_light->GetProperties().HasProperty("LinearAttenuation")) {
      new_light->setLinearAttenuation(map_light->GetProperties().GetFloatProperty("LinearAttenuation"));
    }
    if(map_light->GetProperties().HasProperty("QuadraticAttenuation")) {
      new_light->setQuadraticAttenuation(map_light->GetProperties().GetFloatProperty("QuadraticAttenuation"));
    }
    if(map_light->GetProperties().HasProperty("ConeAngle")) {
      new_light->setConeAngle(map_light->GetProperties().GetFloatProperty("ConeAngle"));
    }
    if(map_light->GetProperties().HasProperty("ConeDirection")) {
      new_light->setConeDirection(Utility::stringToVec3(map_light->GetProperties().GetStringProperty("ConeDirection"))); 
    }

    new_light->setTransform(std::make_shared<Transform>());
    //Subtract y from height to flip the y coords. Tiled and I do it mirrored.
    new_light->getTransform()->translate(glm::vec3(map_light->GetEllipse()->GetCenterX() / (float)map.GetTileWidth(), (float)map.GetHeight() - map_light->GetEllipse()->GetCenterY() / (float)map.GetTileHeight(), 0.0f));
    if(map_light->GetProperties().HasProperty("ZPosition")) {
      new_light->getTransform()->translate(glm::vec3(0.0f, 0.0f, map_light->GetProperties().GetFloatProperty("ZPosition")));
    }
    lights.push_back(new_light);
  }
  return lights;
}

std::vector<std::shared_ptr<Entity>> SceneGenerator::createStaticallyAnimatedTilesFromMap(const Tmx::Map& map) {
  std::vector<std::shared_ptr<Entity>> animations;
  auto layers = map.GetTileLayers();
  auto tilesets = map.GetTilesets();
  auto path = map.GetFilepath();
  
  unsigned int max_z_order = 0;
  unsigned int min_z_order = 0xFFFFFFFF;

  for(auto layer : layers) {
    if(min_z_order > layer->GetZOrder())
      min_z_order = layer->GetZOrder();
    if(max_z_order < layer->GetZOrder())
      max_z_order = layer->GetZOrder();
  }
  
  for(auto layer : layers) {
    for(int y = 0; y < layer->GetHeight(); y++) {
      for(int x = 0; x < layer->GetWidth(); x++) {
        if(layer->GetTileTilesetIndex(x, y) >= 0) {
          auto tileset = tilesets[layer->GetTileTilesetIndex(x, y)];

          auto tile = tileset->GetTile(layer->GetTileId(x, y));
          auto texture = textureFromTileset(tileset, path);
          unsigned int unit = 0;

          if(tile != nullptr && (!tile->GetProperties().HasProperty("AnimatedSprite") || tile->GetProperties().GetStringProperty("AnimatedSprite") == "False")) {
            auto renderable = Graphics::Renderable::create(generateBasisTile(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight(), 0, 0, -(min_z_order + max_z_order - (float)layer->GetZOrder()) - 1.0));
            auto animator = Graphics::TileAnimator::create(texture->getWidth(), texture->getHeight(), tileset->GetTileWidth(), tileset->GetTileHeight());

            auto frames = tile->GetFrames();

            renderable->addTexture(unit, "tileset0", texture);
            
            for(auto frame : frames) {
              auto id = frame.GetTileID();
              auto duration = frame.GetDuration();

              int width_in_tiles = texture->getWidth() / tileset->GetTileWidth();
              int height_in_tiles = texture->getHeight() / tileset->GetTileHeight();
              
              int x_pos = id % width_in_tiles;
              int y_pos = height_in_tiles - 1 - id / width_in_tiles;

              animator->addFrameBack("default", glm::ivec2(x_pos, y_pos), duration, true);
            }

            renderable->setShader((*shader_manager.lock())["tile_animation"]);
            
            //subtract y from layer height, and then subtract an additional 1 to normalize it to 0
            std::shared_ptr<Entity> entity = std::make_shared<Entity>();
            entity->addComponent(renderable);
            entity->addComponent(animator);

            entity->getTransform()->translate(glm::vec3((float)x, layer->GetHeight() - (float)y - 1.0, 0.0f));
            if(layer->IsVisible()) 
              entity->setActive(true);
            animations.push_back(entity);
          }
        }
      }
    }
  }

  return animations;
}

SceneGenerator::MapRenderables SceneGenerator::createRenderablesFromMap(const unsigned int patch_width_tiles, const unsigned int patch_height_tiles, const Tmx::Map& map) {
  MapRenderables renderables;
  auto layers = map.GetTileLayers();
  auto tilesets = map.GetTilesets();
  auto path = map.GetFilepath();

  //find the min, max z in the given layers
  unsigned int max_z_order = 0;
  unsigned int min_z_order = 0xFFFFFFFF;

  for(auto layer : layers) {
    if(min_z_order > layer->GetZOrder()) {
      min_z_order = layer->GetZOrder();
    }
    if(max_z_order < layer->GetZOrder()) {
      max_z_order = layer->GetZOrder();
    }
  }

  unsigned int width_in_patches = ceil((float)map.GetWidth() / (float)patch_width_tiles);
  unsigned int height_in_patches = ceil((float)map.GetHeight() / (float)patch_height_tiles);

  for(auto layer : layers) {
    auto layer_entity = std::make_shared<Entity>();
    layer_entity->setActive(true);

    for(unsigned int patch_y = 0; patch_y < height_in_patches; patch_y++) {
      for(unsigned int patch_x = 0; patch_x < width_in_patches; patch_x++) {
        std::vector<glm::vec3> patch_vertices;
        std::vector<glm::vec2> patch_texture_coords;
        std::vector<int> patch_texture_units;
        std::vector<std::shared_ptr<Graphics::BaseTexture>> patch_textures;
        std::map<int, std::string> patch_texture_names;


        for(unsigned int tile_y = 0; tile_y < patch_height_tiles; tile_y++) {
          for(unsigned int tile_x = 0; tile_x < patch_width_tiles; tile_x++) {

            //Get the actual map-tile coord for calculations
            auto map_x = patch_x * patch_width_tiles + tile_x;
            auto map_y = patch_y * patch_height_tiles + tile_y;
            auto opengl_map_y = map.GetHeight() - map_y - 1;

            
            //If the generated map tile coord is out of bounds for the map, discard this iteration
            //This will happen if ASSERT(map.getWidth() % patch_width_tiles != 0 || map.getHeight() % patch_height_tiles != 0]
            if(map_x >= map.GetWidth() || map_y >= map.GetHeight())
              continue;

            //Retrieve tileset and tile from layer
            auto tileset_index = layer->GetTileTilesetIndex(map_x, map_y);

            //If there is no tile here on this layer, discard this iteration
            if(tileset_index < 0) {
              continue;
            }

            //Calculate OpenGL compatible z-order
            auto z_order = -(min_z_order + max_z_order - (float)layer->GetZOrder()) - 1.0;

            auto tileset = tilesets[tileset_index];
            auto tile = tileset->GetTile(layer->GetTileId(map_x, map_y));


            //Check if this tile has an animated sprite on it, in which case this is taken care of elsewhere, discard this iteration
            if(tile != nullptr && tile->GetProperties().GetStringProperty("AnimatedSprite") == "True") {
              AnimationPlaceholder placeholder;
              placeholder.sprite_name = tile->GetProperties().GetStringProperty("CharacterName");
              placeholder.default_animation = tile->GetProperties().GetStringProperty("DefaultAnimation");
              placeholder.x_pos = map_x;
              placeholder.y_pos = opengl_map_y;
              placeholder.z_order = z_order;
              renderables.dynamic_animations.push_back(placeholder);
            }
            else if(!tile) {
              //Generate Vertex Coords
              auto vertex_coords = generateVertexCoords(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight(), map_x, opengl_map_y, z_order);
              patch_vertices.insert(patch_vertices.end(), vertex_coords.begin(), vertex_coords.end());
              
              //Generate Textures
              auto next_texture = textureFromTileset(tileset, path);
              int texture_unit;
              
              //See if texture already exists on patch
              auto found_texture = std::find(patch_textures.begin(), patch_textures.end(), next_texture);

              //If it doesn't exist, grab the current number of patch textures for the new texture unit and add the next texture
              if(found_texture == patch_textures.end()) {
                texture_unit = patch_textures.size();
                patch_textures.push_back(next_texture);

                //Set the appropriate sampler name for this tileset
                std::stringstream sampler_name;
                sampler_name << "tileset" << texture_unit;
                patch_texture_names[texture_unit] = sampler_name.str();
              }
              //If it does exist, calculate the texture unit
              else {
                texture_unit = (int)(found_texture - patch_textures.begin());
              }
              
              //Generate Texture Unit Vector
              for(int i = 0; i < 6; i++) {
                patch_texture_units.push_back(texture_unit);
              }

              //Generate Texture Coords
              auto tex_coords = generateTextureCoords(layer, map_x, map_y, next_texture->getWidth(), next_texture->getHeight(), tileset->GetTileWidth(), tileset->GetTileHeight());
              patch_texture_coords.insert(patch_texture_coords.end(), tex_coords.begin(), tex_coords.end());

            }
          }
        }

        //If this patch is actually supposed to exist
        if(patch_vertices.size() > 0 && patch_texture_coords.size() > 0 && patch_texture_units.size() > 0) {
          //Create vertex data
          Graphics::VertexData patch_vertex_data(GL_TRIANGLES);
          patch_vertex_data.addVec(Graphics::VertexData::DATA_TYPE::GEOMETRY, patch_vertices);
          patch_vertex_data.addVec(Graphics::VertexData::DATA_TYPE::TEX_COORDS, patch_texture_coords);
          patch_vertex_data.addVec(Graphics::VertexData::DATA_TYPE::TEXTURE_UNIT, patch_texture_units);

          //Create renderable and populate it with data
          auto renderable = Graphics::Renderable::create(patch_vertex_data);
          for(auto texture_index = 0; texture_index < patch_textures.size(); texture_index++) {
            renderable->addTexture(texture_index, patch_texture_names[texture_index], patch_textures[texture_index]);
          }

          //Check if this map is lighted
          //If it is, give the renderable a diffuse shader, set it's ambient color and intensity, and set it to react to lights
          if(map.GetProperties().HasProperty("Lighted") && map.GetProperties().GetStringProperty("Lighted") == "True") {
            renderable->setShader((*shader_manager.lock())["diffuse_lighting"]);
            renderable->setLightReactive(true);
            if(map.GetProperties().HasProperty("AmbientColor"))
              renderable->setAmbientLight(Utility::stringToVec3(map.GetProperties().GetStringProperty("AmbientColor")) / glm::vec3(256.0, 256.0, 256.0));
            if(map.GetProperties().HasProperty("AmbientIntensity"))
              renderable->setAmbientIntensity(map.GetProperties().GetFloatProperty("AmbientIntensity"));
          }
          //If it isn't, it just needs a simple texturing shader
          else {
            renderable->setShader((*shader_manager.lock())["simple_texture"]); 
          }

          if(layer->IsVisible()) {
            renderable->setActive(true);
          }
          else {
            renderable->setActive(false);
          }

          layer_entity->addComponent(renderable);

        }
      }
    }
    renderables.entities.push_back(layer_entity);
  }
  return renderables;
}

std::map<std::string, SceneGenerator::DynamicAnimation> SceneGenerator::createAnimationsFromAnimationMap(const Tmx::Map& map) {
  std::map<std::string, DynamicAnimation> animations; 
  auto path = map.GetFilepath();
  for(auto& tileset : map.GetTilesets()) {
    for(auto& tile : tileset->GetTiles()) {
      auto properties = tile->GetProperties();

      if(properties.HasProperty("SpriteName") && properties.HasProperty("AnimationName")) {
        auto sprite_name = properties.GetStringProperty("SpriteName");

        if(animations.count(sprite_name) == 0) {
          DynamicAnimation anim;

          auto texture = textureFromTileset(tileset, path);
          auto renderable = Graphics::Renderable::create(generateBasisTile(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight()));
          unsigned int unit = 0;

          renderable->addTexture(unit, "tileset0", texture);
          renderable->setShader((*shader_manager.lock())["tile_animation"]);

          anim.entity = std::make_shared<Entity>();
          anim.entity->addComponent(renderable);

          animations.insert(std::pair<std::string, DynamicAnimation>(sprite_name, anim));
        }

        auto animation_name = properties.GetStringProperty("AnimationName");

        auto frames = tile->GetFrames();
        if(animations[sprite_name].animator == nullptr) {
          animations[sprite_name].animator = Graphics::TileAnimator::create(tileset->GetImage()->GetWidth(), tileset->GetImage()->GetHeight(), tileset->GetTileWidth(), tileset->GetTileHeight());
          animations[sprite_name].entity->addComponent(animations[sprite_name].animator);
        }
         
        for(auto frame : frames) {
          auto id = frame.GetTileID();
          auto duration = frame.GetDuration();

          int width_in_tiles = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
          int height_in_tiles = tileset->GetImage()->GetHeight() / tileset->GetTileHeight();
          
          int x_pos = id % width_in_tiles;
          int y_pos = height_in_tiles - 1 - id / width_in_tiles;

          animations[sprite_name].animator->addFrameBack(animation_name, glm::ivec2(x_pos, y_pos), duration);
        }
      }
    }
  }
  return animations;
}