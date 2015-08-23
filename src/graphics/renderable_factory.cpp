#include <easylogging++.h>
#include <glm/ext.hpp>
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
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<AnimatedTile>(new_binding, generateTile());
  }

  template<>
  std::shared_ptr<Renderable> RenderableFactory::create(const VertexData& vertex_data) {
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<Renderable>(new_binding, vertex_data);
  }

  template<>
  std::shared_ptr<Tile> RenderableFactory::create(const VertexData& vertex_data) {
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<Tile>(new_binding, vertex_data);
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create(const VertexData& vertex_data) {
    unsigned int new_binding = 0;
    glGenVertexArrays(1, &new_binding);
    glBindVertexArray(new_binding);
    glBindVertexArray(0);
    return std::make_shared<AnimatedTile>(new_binding, vertex_data);
  }

  std::vector<std::shared_ptr<Renderable>> RenderableFactory::createFromMap(const Tmx::Map& map, TextureManager& texture_manager, const ShaderManager& shader_manager) {
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
    
    //Get out of the map directory
    auto pos = path.find_last_of("/");
    path = path.substr(0, pos);
    pos = path.find_last_of("/");
    path = path.substr(0, pos);

    for(auto layer : layers) {
      for(int y = 0; y < layer->GetHeight(); y++) {
        for(int x = 0; x < layer->GetWidth(); x++) {
          if(layer->GetTileTilesetIndex(x, y) >= 0) {
            auto tileset = tilesets[layer->GetTileTilesetIndex(x, y)];

            auto tile = tileset->GetTile(layer->GetTileId(x, y));
            auto tileset_image = tileset->GetImage();

            auto source = tileset_image->GetSource();
            //add path to the image source
            source.erase(0, 2);
            source = path + source;

            auto texture_name = TextureManager::getNameFromPath(source);

            if(!texture_manager.textureExists(texture_name)) {
              if(!texture_manager.loadTexture(source, "tileset")) {
                throw Exceptions::InvalidFilenameException(source);
              }
            }
            auto texture = texture_manager[texture_name];
            
            if(!tile) {
              auto id = layer->GetTileId(x, y);
              std::vector<glm::vec3> verts {
                glm::vec3(0.0, 0.0, -1.0),
                glm::vec3(0.0, 1.0, -1.0),
                glm::vec3(1.0, 1.0, -1.0),
                glm::vec3(1.0, 0.0, -1.0)
              };

              std::vector<glm::vec2> texs {
                glm::vec2(-0.5, -0.5),
                glm::vec2(-0.5, 0.5),
                glm::vec2(0.5, 0.5),
                glm::vec2(0.5, -0.5)
              };
              
              //Tile flipping
              if(layer->IsTileFlippedDiagonally(x, y) && layer->IsTileFlippedHorizontally(x, y) && layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Reflect across x=y
                  texs[i] = texs[i] * glm::mat2(0.0, 1.0, 1.0, 0.0);
                }
              }
              else if(layer->IsTileFlippedDiagonally(x, y) && layer->IsTileFlippedHorizontally(x, y) && !layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Rotate 90 Anticlockwise
                  texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
                }               
              }
              else if(layer->IsTileFlippedDiagonally(x, y) && !layer->IsTileFlippedHorizontally(x, y) && layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Rotate 90 Clockwise
                  texs[i] = texs[i] * glm::mat2(0.0, 1.0, -1.0, 0.0);
                }       
              }
              else if(!layer->IsTileFlippedDiagonally(x, y) && layer->IsTileFlippedHorizontally(x, y) && layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Rotate 180
                  texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, -1.0);
                }       
              }
              else if(layer->IsTileFlippedDiagonally(x, y) && !layer->IsTileFlippedHorizontally(x, y) && !layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //rotate anticlockwise 90, reflect across x
                  texs[i] = texs[i] * glm::mat2(0.0, -1.0, 1.0, 0.0);
                  texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
                }       
              }
              else if(!layer->IsTileFlippedDiagonally(x, y) && !layer->IsTileFlippedHorizontally(x, y) && layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Reflect across x
                  texs[i] = texs[i] * glm::mat2(1.0, 0.0, 0.0, -1.0);
                }       
              }
              else if(!layer->IsTileFlippedDiagonally(x, y) && layer->IsTileFlippedHorizontally(x, y) && !layer->IsTileFlippedVertically(x, y)) {
                for(int i = 0; i < 4; i++) {
                  //Reflect across y
                  texs[i] = texs[i] * glm::mat2(-1.0, 0.0, 0.0, 1.0);
                }       
              }
              
              //Translate to bottom left at 0,0
              for(int i = 0; i < 4; i++) {
                texs[i] = texs[i] + glm::vec2(0.5, 0.5);
              }

              float width = (float)tileset->GetTileWidth() / (float)tileset_image->GetWidth();
              float height = (float)tileset->GetTileHeight() / (float)tileset_image->GetHeight();
               
              //scale by width/height
              for(int i = 0; i < 4; i++) {
                texs[i] = texs[i] * glm::vec2(width, height);
              }

              int width_in_tiles = tileset_image->GetWidth() / tileset->GetTileWidth();
              int height_in_tiles = tileset_image->GetHeight() / tileset->GetTileHeight();
              int x_pos = id % width_in_tiles;
              int y_pos = height_in_tiles - 1 - id / width_in_tiles;

              //translate by pos
              for(int i = 0; i < 4; i++)  {
                texs[i] += glm::vec2(x_pos * width, y_pos * height);
              }

              std::vector<unsigned int> indices {
                0, 1, 2, 0, 2, 3
              };
              VertexData vert_data(GL_TRIANGLES);
              vert_data.addIndices(indices);
              vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
              vert_data.addVec<glm::vec2>(VertexData::DATA_TYPE::TEX_COORDS, texs);

              auto renderable = create<Tile>(vert_data);
              renderable->setTexture(texture);
              renderable->setSizeInPixels(map.GetTileWidth());
              renderable->setShader(shader_manager["simple_texture"]);

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
              animated_renderable->setSizeInPixels(map.GetTileWidth());
              
              for(auto frame : frames) {
                auto id = frame.GetTileID();
                auto duration = frame.GetDuration();

                int width_in_tiles = texture->getWidth() / tileset->GetTileWidth();
                int height_in_tiles = texture->getHeight() / tileset->GetTileHeight();
                
                int x_pos = id % width_in_tiles;
                int y_pos = height_in_tiles - 1 - id / width_in_tiles;

                animated_renderable->addFrameBack(glm::ivec2(x_pos, y_pos), duration);
              }

              animated_renderable->setShader(shader_manager["tile_animation"]);
              
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