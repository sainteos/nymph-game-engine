#include "graphics/renderable_factory.h"
#include "graphics/renderable.h"
#include "graphics/tile.h"
#include "graphics/animated_tile.h"

namespace Graphics {
  RenderableFactory::RenderableFactory() : tile_vertex_array_binding(0) {
    glGenVertexArrays(1, &tile_vertex_array_binding); 
    glBindVertexArray(tile_vertex_array_binding);
    glBindVertexArray(0);
  }

  RenderableFactory::~RenderableFactory() {
    glDeleteVertexArrays(1, &tile_vertex_array_binding);
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
      glm::vec3(0.0, 0.0, -1.0),
      glm::vec3(0.0, 1.0, -1.0),
      glm::vec3(1.0, 1.0, -1.0),
      glm::vec3(1.0, 0.0, -1.0)
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
    return std::make_shared<Tile>(tile_vertex_array_binding, generateTile());
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create() {
    return std::make_shared<AnimatedTile>(tile_vertex_array_binding, generateTile());
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
    return std::make_shared<Tile>(tile_vertex_array_binding, vertex_data);
  }

  template<>
  std::shared_ptr<AnimatedTile> RenderableFactory::create(const VertexData& vertex_data) {
    return std::make_shared<AnimatedTile>(tile_vertex_array_binding, vertex_data);
  }
}