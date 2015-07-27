
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
#include "graphics/shader.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;
unsigned int generate_valid_fragment() {
  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  std::string fragment_code = std::string(R"(
  #version 330

  out vec4 fragColor;

  void main()
  {
      fragColor = vec4(1.0, 0.0, 0.0, 1.0);
  })");

  const char *fc_str = fragment_code.c_str(); 
  glShaderSource(fragment_shader, 1, &fc_str, new int[1] {(int)fragment_code.size()});  
  glCompileShader(fragment_shader);

  return fragment_shader;
}

unsigned int generate_valid_vertex() {
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertex_code = std::string(R"(
    #version 330

    layout(location = 0)in vec3 vert;
    uniform float dick;

    void main()
    {
        gl_Position = vec4(vert, 1.0);
    })");

  const char *vc_str = vertex_code.c_str();
  glShaderSource(vertex_shader, 1, &vc_str, new int[1] {(int)vertex_code.size()});
  glCompileShader(vertex_shader);
  
  return vertex_shader;
}


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
  std::vector<unsigned int> indices {
    0, 1, 2, 0, 2, 3
  };
  VertexData vert_data(GL_TRIANGLES);
  vert_data.addIndices(indices);
  vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);

  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  std::shared_ptr<Shader> shader_ptr = std::make_shared<Shader>(generate_valid_vertex(), generate_valid_fragment());

  std::shared_ptr<Renderable> renderable_ptr(new Renderable(good_binding, vert_data));
  renderable_ptr->setShader(shader_ptr);
  renderable_ptr->initialize();
  renderable_ptr->setActive();
  graphics.addRenderable(renderable_ptr);
  //renderable_ptr->getShader()->setUniform<float>("dick", 10.0f);

  glViewport(0, 0, 800, 800);
  graphics.renderLoop();
  graphics.destroy();

  return 0;
}
