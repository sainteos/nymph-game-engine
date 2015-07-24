#include <easylogging++.h>
#include <catch.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "graphics/shader.h"
#include "exceptions/invalid_fragment_shader_exception.h"
#include "exceptions/invalid_vertex_shader_exception.h"
#include "opengl_setup.h"

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

    void main()
    {
        gl_Position = vec4(vert, 1.0);
    })");

  const char *vc_str = vertex_code.c_str();
  glShaderSource(vertex_shader, 1, &vc_str, new int[1] {(int)vertex_code.size()});
  glCompileShader(vertex_shader);
  
  return vertex_shader;
}

SCENARIO("Shader can be created with a valid fragment and valid vertex shader") {
  setup_opengl();
  GIVEN("Valid fragment and vertex programs") {
    unsigned int frag = generate_valid_fragment();
    unsigned int vert = generate_valid_vertex();
    WHEN("A shader object is created with them") {
      Graphics::Shader shader(vert, frag);
      THEN("The shader object's handle should be valid") {
        REQUIRE(glIsProgram(shader.getHandle()));
      }
    }
  }
  destroy_opengl();
}

SCENARIO("Shader can't be created with an invalid fragment and valid vertex shader") {
  setup_opengl();
  GIVEN("Invalid fragment and valid vertex programs") {
    unsigned int frag = 190839859;
    unsigned int vert = generate_valid_vertex();
    WHEN("A shader object is created with them") {
      THEN("it should throw an InvalidFragmentShader exception") {
        REQUIRE_THROWS_AS(Graphics::Shader(vert, frag), Exceptions::InvalidFragmentShaderException);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("Shader can't be created with a valid fragment and invalid vertex shader") {
  setup_opengl();
  GIVEN("Valid fragment and invalid vertex programs") {
    unsigned int frag = generate_valid_fragment();
    unsigned int vert = 2839858;
    WHEN("A shader object is created with them") {
      THEN("it should throw an InvalidVertexShader exception") {
        REQUIRE_THROWS_AS(Graphics::Shader(vert, frag), Exceptions::InvalidVertexShaderException);
      }
    }
  }
  destroy_opengl();
}