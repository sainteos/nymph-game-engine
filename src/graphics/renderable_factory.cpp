#include <easylogging++.h>
#include <glm/ext.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include <algorithm>
#include "graphics/renderable_factory.h"
#include "graphics/renderable.h"
#include "graphics/tile.h"
#include "graphics/animated_tile.h"
#include "transform.h"
#include "exceptions/invalid_filename_exception.h"
#include "graphics/light.h"
#include "utility/utility_functions.h"

namespace Graphics {
  RenderableFactory::RenderableFactory() {
  }

  RenderableFactory::~RenderableFactory() {
  }

  const VertexData RenderableFactory::generateCube() {
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

    VertexData vertex_data(GL_QUADS);
    vertex_data.addIndices(indices);
    vertex_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);

    return vertex_data;
  }

  const VertexData RenderableFactory::generateTile(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int offset_x, const unsigned int offset_y) {
    std::vector<glm::vec2> texs {
      glm::vec2(0.0, 0.0),
      glm::vec2(0.0, 1.0),
      glm::vec2(1.0, 1.0),
      glm::vec2(1.0, 0.0)
    };
    std::vector<unsigned int> indices {
      0, 1, 2, 0, 2, 3
    };
    VertexData vert_data(GL_TRIANGLES);
    vert_data.addIndices(indices);
    vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, generateVertexCoords(base_width, base_height, current_width, current_height, offset_x, offset_y));
    vert_data.addVec<glm::vec2>(VertexData::DATA_TYPE::TEX_COORDS, texs);

    return vert_data;
  }

  unsigned int RenderableFactory::generateVertexArrayObject(VertexData vertex_data) {
    LOG(INFO)<<"Renderable initializing...";

    unsigned int vertex_array_object = 0;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    auto float_data = vertex_data.getCollapsedVectors<float>();
    auto double_data = vertex_data.getCollapsedVectors<double>();
    auto int_data = vertex_data.getCollapsedVectors<int>();
    auto unsigned_int_data = vertex_data.getCollapsedVectors<unsigned int>();
    
    unsigned int num_of_vertex_buffers = vertex_data.numberVertexBufferObjects();
    unsigned int* vertex_buffer_objects = new unsigned int[num_of_vertex_buffers];
    unsigned int index_buffer_object = 0;

    std::vector<std::pair<VertexData::DATA_TYPE, GLenum>> data_types;
    glGenBuffers(num_of_vertex_buffers, vertex_buffer_objects);

    unsigned int current_buffer = 0;
    
    //Do this if we actually have indices
    if(vertex_data.getIndices().size() > 0) {
      glGenBuffers(1, &index_buffer_object);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_data.getIndices().size() * sizeof(unsigned int), &(vertex_data.getIndices())[0], GL_STATIC_DRAW);
    }

    for(auto i : float_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(float), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_FLOAT));
    }

    for(auto i : double_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(double), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_DOUBLE));
    }

    for(auto i : int_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(int), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_INT));
    }

    for(auto i : unsigned_int_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(unsigned int), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_UNSIGNED_INT));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vertex_array_object);

    for(int i = 0; i < num_of_vertex_buffers; i++) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[i]);
      glVertexAttribPointer(data_types[i].first, VertexData::DataWidth.at(data_types[i].first), data_types[i].second, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(data_types[i].first);

    }
    
    if(vertex_data.getIndices().size() > 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    }
    glBindVertexArray(0);

    return vertex_array_object;
  }

  std::shared_ptr<BaseTexture> RenderableFactory::textureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name) {
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

    auto texture_name = TextureManager::getNameFromPath(source);

    if(!texture_manager.textureExists(texture_name)) {
      if(!texture_manager.loadTexture(source, uniform_name, 0)) {
        throw Exceptions::InvalidFilenameException(source);
      }
    }

    return texture_manager[texture_name];
  }

  std::shared_ptr<BaseTexture> RenderableFactory::normalTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name) {
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

    auto texture_name = TextureManager::getNameFromPath(source);

    if(!texture_manager.textureExists(texture_name)) {
      if(!texture_manager.loadTexture(source, uniform_name, 1)) {
        throw Exceptions::InvalidFilenameException(source);
      }
    }

    return texture_manager[texture_name];
  }

  std::shared_ptr<BaseTexture> RenderableFactory::displacementTextureFromTileset(const Tmx::Tileset* tileset, TextureManager& texture_manager, const std::string& path, const std::string& uniform_name) {
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

    auto texture_name = TextureManager::getNameFromPath(source);

    if(!texture_manager.textureExists(texture_name)) {
      if(!texture_manager.loadTexture(source, uniform_name, 2)) {
        throw Exceptions::InvalidFilenameException(source);
      }
    }

    return texture_manager[texture_name];
  }

  std::vector<glm::vec2> RenderableFactory::generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height, const unsigned int tile_width, const unsigned int tile_height) {
    std::vector<glm::vec2> texs {
      glm::vec2(-0.5, -0.5),
      glm::vec2(-0.5, 0.5),
      glm::vec2(0.5, 0.5),
      glm::vec2(0.5, -0.5)
    };
    
    //Tile flipping
    if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Reflect across x=y
        texs[i] = texs[i] * glm::mat2(0.0, 1.0, 1.0, 0.0);
      }
    }
    else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Rotate 90 Anticlockwise
        texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
      }               
    }
    else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Rotate 90 Clockwise
        texs[i] = texs[i] * glm::mat2(0.0, 1.0, -1.0, 0.0);
      }       
    }
    else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Rotate 180
        texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, -1.0);
      }       
    }
    else if(layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //rotate anticlockwise 90, reflect across x
        texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
        texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
      }       
    }
    else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && !layer->IsTileFlippedHorizontally(x_pos, y_pos) && layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Reflect across x
        texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
      }       
    }
    else if(!layer->IsTileFlippedDiagonally(x_pos, y_pos) && layer->IsTileFlippedHorizontally(x_pos, y_pos) && !layer->IsTileFlippedVertically(x_pos, y_pos)) {
      for(int i = 0; i < 4; i++) {
        //Reflect across y
        texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, 1.0);
      }       
    }
    
    //Translate to bottom left at 0,0
    for(int i = 0; i < 4; i++) {
      texs[i] = texs[i] + glm::vec2(0.5, 0.5);
    }

    float width = (float)tile_width / (float)texture_width;
    float height = (float)tile_height / (float)texture_height;
     
    //scale by width/height
    for(int i = 0; i < 4; i++) {
      texs[i] = texs[i] * glm::vec2(width, height);
    }

    auto id = layer->GetTileId(x_pos, y_pos);

    int width_in_tiles = texture_width / tile_width;
    int height_in_tiles = texture_height / tile_height;
    int tileset_x_pos = id % width_in_tiles;
    int tileset_y_pos = height_in_tiles - 1 - id / width_in_tiles;

    //translate by pos
    for(int i = 0; i < 4; i++)  {
      texs[i] += glm::vec2(tileset_x_pos * width, tileset_y_pos * height);
    }

    return texs;
  }

  std::vector<glm::vec3> RenderableFactory::generateVertexCoords(const unsigned int base_width, const unsigned int base_height, const unsigned int current_width, const unsigned int current_height, const unsigned int offset_x, const unsigned int offset_y) {
    std::vector<glm::vec3> verts {
      glm::vec3((float)offset_x / (float)base_width, (float)offset_y / (float)base_height, 0.0),
      glm::vec3((float)offset_x / (float)base_width, (float)offset_y / (float)base_height + (float)current_height / (float)base_height, 0.0),
      glm::vec3((float)offset_x / (float)base_width + (float)current_width / (float)base_width, (float)offset_y / (float)base_height + (float)current_height / (float)base_height, 0.0),
      glm::vec3((float)offset_x / (float)base_width + (float)current_width / (float)base_width, (float)offset_y / (float)base_height, 0.0)
    };
    return verts;
  }

  template<>
  std::shared_ptr<Renderable> RenderableFactory::create() {
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<Renderable>(new_binding, generateCube());
  }

  template<>
  std::shared_ptr<Tile> RenderableFactory::create() {
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<Tile>(new_binding, generateTile(32, 32, 32, 32));
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create() {
    if(!glIsVertexArray(animated_tile_vao)) {
      animated_tile_vao = generateVertexArrayObject(generateTile(32, 32, 32, 32));
    }
    return std::make_shared<AnimatedTile>(animated_tile_vao, generateTile(32, 32, 32, 32));
  }

  template<>
  std::shared_ptr<Renderable> RenderableFactory::create(const VertexData& vertex_data) {
    return std::make_shared<Renderable>(generateVertexArrayObject(vertex_data), vertex_data);
  }

  template<>
  std::shared_ptr<Tile> RenderableFactory::create(const VertexData& vertex_data) {
    return std::make_shared<Tile>(generateVertexArrayObject(vertex_data), vertex_data);
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create(const VertexData& vertex_data) {
    return std::make_shared<AnimatedTile>(generateVertexArrayObject(vertex_data), vertex_data);
  }

  std::vector<std::shared_ptr<Light>> RenderableFactory::createLightsFromMap(const Tmx::Map& map) {
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<Tmx::Object*> light_map_objects;
    for(auto group : map.GetObjectGroups()) {
      std::copy_if(group->GetObjects().begin(), group->GetObjects().end(), std::back_inserter(light_map_objects),
        [&](Tmx::Object* o) {
          return o->GetProperties().HasProperty("Type") && o->GetProperties().GetStringProperty("Type") == "Light";
        }
      );
    }

    for(auto map_light : light_map_objects) {
      Light::Type light_type = Light::Type::POINT;
      if(map_light->GetProperties().HasProperty("LightType")) {
        light_type = Light::stringToType(map_light->GetProperties().GetStringProperty("LightType"));
      }
      
      std::shared_ptr<Light> new_light = std::make_shared<Light>(light_type);
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

  std::vector<std::shared_ptr<AnimatedTile>> RenderableFactory::createStaticallyAnimatedTilesFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager) {
    std::vector<std::shared_ptr<AnimatedTile>> tiles;
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
            auto texture = textureFromTileset(tileset, texture_manager, path, "tileset");

            if(tile != nullptr && (!tile->GetProperties().HasProperty("AnimatedSprite") || tile->GetProperties().GetStringProperty("AnimatedSprite") == "False")) {
              auto animated_renderable = create<AnimatedTile>(generateTile(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight()));
              auto frames = tile->GetFrames();

              animated_renderable->addTexture(texture);
              animated_renderable->setSizeInPixels(tileset->GetTileWidth());
              
              for(auto frame : frames) {
                auto id = frame.GetTileID();
                auto duration = frame.GetDuration();

                int width_in_tiles = texture->getWidth() / tileset->GetTileWidth();
                int height_in_tiles = texture->getHeight() / tileset->GetTileHeight();
                
                int x_pos = id % width_in_tiles;
                int y_pos = height_in_tiles - 1 - id / width_in_tiles;

                animated_renderable->addFrameBack(glm::ivec2(x_pos, y_pos), duration);
              }

              animated_renderable->setShader((*shader_manager)["tile_animation"]);
              
              //subtract y from layer height, and then subtract an additional 1 to normalize it to 0
              animated_renderable->getTransform()->translate(glm::vec3((float)x, layer->GetHeight() - (float)y - 1.0, -(min_z_order + max_z_order - (float)layer->GetZOrder()) - 1.0));
              if(layer->IsVisible())
                animated_renderable->setActive();
              tiles.push_back(animated_renderable);
            }
          }
        }
      }
    }

    return tiles;
  }

  MapRenderables RenderableFactory::createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager) {
    std::vector<RenderableInfo> gid_to_vao;
    MapRenderables renderables;
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
            auto texture = textureFromTileset(tileset, texture_manager, path, "tileset");
            std::shared_ptr<BaseTexture> normal_texture;
            if(tileset->GetProperties().HasProperty("UseNormal") && tileset->GetProperties().GetStringProperty("UseNormal") == "True") {
              normal_texture = normalTextureFromTileset(tileset, texture_manager, path, "normal_texture");
            }

            if(tile != nullptr && tile->GetProperties().GetStringProperty("AnimatedSprite") == "True") {
              AnimationPlaceholder placeholder;
              placeholder.sprite_name = tile->GetProperties().GetStringProperty("CharacterName");
              placeholder.default_animation = tile->GetProperties().GetStringProperty("DefaultAnimation");
              placeholder.x_pos = x;
              placeholder.y_pos = layer->GetHeight() - y - 1;
              placeholder.z_order = -(min_z_order + max_z_order - (float)layer->GetZOrder()) - 1.0;
              renderables.dynamic_animations.push_back(placeholder);
            }
            else if(!tile) {
              unsigned int vertex_array_object = 0;
              VertexData vert_data(GL_TRIANGLES);

              auto id = layer->GetTileId(x, y);

              auto find_pred = [&](const RenderableInfo& info) { return layer->GetTileGid(x,y) == info.gid &&
                                                                        layer->IsTileFlippedHorizontally(x, y) == info.flip_horizontal &&
                                                                        layer->IsTileFlippedVertically(x, y) == info.flip_vertical &&
                                                                        layer->IsTileFlippedDiagonally(x, y) == info.flip_diagonal; };
              auto iter = std::find_if(gid_to_vao.begin(), gid_to_vao.end(), find_pred);

              if(iter == gid_to_vao.end()) {
                std::vector<unsigned int> indices {
                  0, 1, 2, 0, 2, 3
                };
                std::vector<glm::vec3> verts;
                verts = generateVertexCoords(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight());
                std::vector<glm::vec2> texs;
                texs = generateTextureCoords(layer, x, y, texture->getWidth(), texture->getHeight(), tileset->GetTileWidth(), tileset->GetTileHeight());
                vert_data.addIndices(indices);
                vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
                vert_data.addVec<glm::vec2>(VertexData::DATA_TYPE::TEX_COORDS, texs);

                vertex_array_object = generateVertexArrayObject(vert_data);

                RenderableInfo info { vert_data, vertex_array_object, layer->GetTileGid(x, y) };
                info.flip_horizontal = layer->IsTileFlippedHorizontally(x, y);
                info.flip_vertical = layer->IsTileFlippedVertically(x, y);
                info.flip_diagonal = layer->IsTileFlippedDiagonally(x, y);
                gid_to_vao.push_back(info);
              }
              else {
                vertex_array_object = iter->vertex_array_object;
                vert_data = iter->vertex_data;
              }

              std::shared_ptr<Tile> renderable = std::make_shared<Tile>(vertex_array_object, vert_data);

              renderable->addTexture(texture);
              renderable->setSizeInPixels(tileset->GetTileWidth());
              if(normal_texture) {
                renderable->addTexture(normal_texture);
              }

              if(map.GetProperties().HasProperty("Lighted") && map.GetProperties().GetStringProperty("Lighted") == "True") {
                renderable->setShader((*shader_manager)["diffuse_lighting"]);
                renderable->setLightReactive(true);
                if(map.GetProperties().HasProperty("AmbientColor"))
                  renderable->setAmbientLight(Utility::stringToVec3(map.GetProperties().GetStringProperty("AmbientColor")) / glm::vec3(256.0, 256.0, 256.0));
                if(map.GetProperties().HasProperty("AmbientIntensity"))
                  renderable->setAmbientIntensity(map.GetProperties().GetFloatProperty("AmbientIntensity"));
              }
              else {
                renderable->setShader((*shader_manager)["simple_texture"]); 
              }

              //subtract y from height and subtract 1 to normalize to 0
              renderable->getTransform()->translate(glm::vec3((float)x, layer->GetHeight() - (float)y - 1.0, -(min_z_order + max_z_order - (float)layer->GetZOrder()) - 1.0));
              if(layer->IsVisible())
                renderable->setActive();
              renderables.tiles.push_back(renderable);
            }
            else {
            }
          }
        }
      }
    }

    return renderables;
  }

  std::vector<Animation> RenderableFactory::createAnimationsFromAnimationMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager) {
    std::vector<Animation> animations;
    auto path = map.GetFilepath();
    for(auto& tileset : map.GetTilesets()) {
      for(auto& tile : tileset->GetTiles()) {
        auto properties = tile->GetProperties();
        if(properties.HasProperty("SpriteName") && properties.HasProperty("AnimationName")) {
          Animation new_anim; 
          new_anim.sprite_name = properties.GetStringProperty("SpriteName");
          new_anim.animation_name = properties.GetStringProperty("AnimationName");

          auto texture = textureFromTileset(tileset, texture_manager, path, "tileset");

          new_anim.tile = create<AnimatedTile>(generateTile(map.GetTileWidth(), map.GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight()));
          auto frames = tile->GetFrames();

          new_anim.tile->addTexture(texture);
          new_anim.tile->setSizeInPixels(tileset->GetTileWidth());
          
          for(auto frame : frames) {
            auto id = frame.GetTileID();
            auto duration = frame.GetDuration();

            int width_in_tiles = texture->getWidth() / tileset->GetTileWidth();
            int height_in_tiles = texture->getHeight() / tileset->GetTileHeight();
            
            int x_pos = id % width_in_tiles;
            int y_pos = height_in_tiles - 1 - id / width_in_tiles;

            new_anim.tile->addFrameBack(glm::ivec2(x_pos, y_pos), duration);
          }

          new_anim.tile->setShader((*shader_manager)["tile_animation"]);
          animations.push_back(new_anim);
        }
      }
    }
    return animations;
  }

  std::shared_ptr<Text> RenderableFactory::createText(std::shared_ptr<Font> font, const glm::vec4& color) {
    std::shared_ptr<Text> text = std::make_shared<Text>();
    text->setColor(color);
    text->setFont(font);

    for(auto i : font->getCharacters()) {
      std::vector<glm::vec2> texs {
        glm::vec2(0.0, 1.0),
        glm::vec2(0.0, 0.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(1.0, 1.0)
      };
      std::vector<unsigned int> indices {
        0, 1, 2, 0, 2, 3
      };
      std::vector<glm::vec3> verts {
        glm::vec3(i.second.bearing.x, -i.second.size.y + i.second.bearing.y, 0.0),
        glm::vec3(i.second.bearing.x, i.second.size.y - i.second.size.y + i.second.bearing.y, 0.0),
        glm::vec3(i.second.bearing.x + i.second.size.x, i.second.size.y - i.second.size.y + i.second.bearing.y, 0.0),
        glm::vec3(i.second.bearing.x + i.second.size.x, -i.second.size.y + i.second.bearing.y, 0.0)
      };
      VertexData vert_data(GL_TRIANGLES);
      vert_data.addIndices(indices);
      vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
      vert_data.addVec<glm::vec2>(VertexData::DATA_TYPE::TEX_COORDS, texs);
      auto vao = generateVertexArrayObject(vert_data);

      text->addCharacterVertexData(i.first, vert_data, vao);
    }
    return text;
  }
}