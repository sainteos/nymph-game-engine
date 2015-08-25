#include <easylogging++.h>
#include <glm/ext.hpp>
#include <OpenGL/gl3.h>
#include <algorithm>
#include "graphics/renderable_factory.h"
#include "graphics/renderable.h"
#include "graphics/tile.h"
#include "graphics/animated_tile.h"
#include "transform.h"
#include "exceptions/invalid_filename_exception.h"

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

  const VertexData RenderableFactory::generateTile() {
    std::vector<glm::vec3> verts {
      glm::vec3(0.0, 0.0, 0.0),
      glm::vec3(0.0, 1.0, 0.0),
      glm::vec3(1.0, 1.0, 0.0),
      glm::vec3(1.0, 0.0, 0.0)
    };
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
    vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
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
      if(!texture_manager.loadTexture(source, uniform_name)) {
        throw Exceptions::InvalidFilenameException(source);
      }
    }

    return texture_manager[texture_name];
  }

  std::vector<glm::vec2> RenderableFactory::generateTextureCoords(const Tmx::TileLayer* layer, const unsigned int x_pos, const unsigned int y_pos, const unsigned int texture_width, const unsigned int texture_height) {
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

    auto tile_width = layer->mapGetMap()->GetTileWidth();
    auto tile_height = layer->mapGetMap()->GetTileHeight();

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
    return std::make_shared<Tile>(new_binding, generateTile());
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create() {
    if(!glIsVertexArray(animated_tile_vao)) {
      animated_tile_vao = generateVertexArrayObject(generateTile());
    }
    return std::make_shared<AnimatedTile>(animated_tile_vao, generateTile());
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

  std::vector<std::shared_ptr<Renderable>> RenderableFactory::createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const std::shared_ptr<ShaderManager> shader_manager) {
    std::vector<RenderableInfo> gid_to_vao;
    std::vector<std::shared_ptr<Renderable>> renderables;
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
            
            if(!tile) {
              unsigned int vertex_array_object = 0;
              VertexData vert_data(GL_TRIANGLES);

              auto id = layer->GetTileId(x, y);

              auto find_pred = [&](const RenderableInfo& info) { return layer->GetTileGid(x,y) == info.gid &&
                                                                        layer->IsTileFlippedHorizontally(x, y) == info.flip_horizontal &&
                                                                        layer->IsTileFlippedVertically(x, y) == info.flip_vertical &&
                                                                        layer->IsTileFlippedDiagonally(x, y) == info.flip_diagonal; };
              auto iter = std::find_if(gid_to_vao.begin(), gid_to_vao.end(), find_pred);

              if(iter == gid_to_vao.end()) {
                std::vector<glm::vec3> verts {
                  glm::vec3(0.0, 0.0, -1.0),
                  glm::vec3(0.0, 1.0, -1.0),
                  glm::vec3(1.0, 1.0, -1.0),
                  glm::vec3(1.0, 0.0, -1.0)
                };

                std::vector<unsigned int> indices {
                  0, 1, 2, 0, 2, 3
                };
                std::vector<glm::vec2> texs;
                texs = generateTextureCoords(layer, x, y, texture->getWidth(), texture->getHeight());
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
              renderable->setTexture(texture);
              renderable->setSizeInPixels(map.GetTileWidth());
              renderable->setShader((*shader_manager)["simple_texture"]);

              auto transform = std::make_shared<Transform>();
              //subtract y from height and subtract 1 to normalize to 0
              transform->translate(glm::vec3((float)x, layer->GetHeight() - (float)y - 1.0, -(min_z_order + max_z_order - (float)layer->GetZOrder())));
              renderable->setTransform(transform);

              renderables.push_back(renderable);
            }
            else {
              auto animated_renderable = create<AnimatedTile>();
              auto frames = tile->GetFrames();

              animated_renderable->setTexture(texture);
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
              
              auto transform = std::make_shared<Transform>();
              //subtract y from layer height, and then subtract an additional 1 to normalize it to 0
              transform->translate(glm::vec3((float)x, layer->GetHeight() - (float)y - 1.0, -(min_z_order + max_z_order - (float)layer->GetZOrder())));
              animated_renderable->setTransform(transform);
              renderables.push_back(animated_renderable);
            }
          }
        }
      }
    }

    return renderables;
  }
}