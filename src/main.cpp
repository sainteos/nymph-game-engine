
#include <easylogging++.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/tile_attribute_trait.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;

int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  LOG(INFO)<<"Butts";

  GraphicsSystem graphics;
  graphics.initialize(800, 800, "Dick Butts (tm)", Graphics::WindowExitFunctor(), 60.0);

  std::vector<glm::vec3> verts {
    glm::vec3(-0.5, -0.5, 0.0),
    glm::vec3(-0.5, 0.5, 0.0),
    glm::vec3(0.5, 0.5, 0.0),
    glm::vec3(0.5, -0.5, 0.0),
    glm::vec3(0.5, -0.5, 0.0),
    glm::vec3(0.5, -0.5, 0.0)
  };
  std::vector<unsigned int> indices {
    0, 1, 2, 0, 2, 3
  };
  VertexData vert_data(GL_TRIANGLES);
  vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
  vert_data.addIndices(indices);

  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  std::shared_ptr<Renderable> renderable_ptr(new Renderable(good_binding, vert_data));

  renderable_ptr->initialize();
  graphics.addRenderable(renderable_ptr);

  graphics.renderLoop();
  graphics.destroy();

  return 0;
}
