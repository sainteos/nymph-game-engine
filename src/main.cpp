
#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/tile_attribute_trait.h"
#include "graphics/shader.h"
#include "graphics/base_texture.h"
#include "graphics/shader_manager.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;

int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  LOG(INFO)<<"Butts";

  GraphicsSystem graphics;
  graphics.initialize(800, 800, "Dick Butts (tm)", Graphics::WindowExitFunctor(), 60.0);

  std::vector<glm::vec3> verts {
    glm::vec3(-0.5, -0.5, -1.0),
    glm::vec3(-0.5, 0.5, -1.0),
    glm::vec3(0.5, 0.5, -1.0),
    glm::vec3(0.5, -0.5, -1.0)
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

  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  ShaderManager shader_manager;
  shader_manager.loadShader("simple_texture");
  std::shared_ptr<BaseTexture> texture_ptr = std::make_shared<BaseTexture>("sample", GL_TEXTURE_2D, 0);
  texture_ptr->load("./test/textures/test1.png");
  std::shared_ptr<Renderable> renderable_ptr(new Renderable(good_binding, vert_data));
  try {
    renderable_ptr->setShader(shader_manager["simple_texture"]);
  }
  catch(std::exception& e) {
    LOG(ERROR)<<e.what();
    return 0;
  }
  renderable_ptr->addTexture(texture_ptr);
  renderable_ptr->initialize();
  renderable_ptr->setActive();
  graphics.addRenderable(renderable_ptr);
  renderable_ptr->setTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.3, -0.3, 0.0)));
  glViewport(0, 0, 800, 800);
  graphics.renderLoop();
  graphics.destroy();

  return 0;
}
