#include <catch.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "graphics/renderable.h"
#include "exceptions/invalid_vertex_array_exception.h"

void setup_opengl() {
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  auto window = glfwCreateWindow(400, 400, "Renderable test window", nullptr, nullptr);
  glfwMakeContextCurrent(window);
}

void destroy_opengl() {
  glfwTerminate();
}

SCENARIO("A renderable can't be constructed with an invalid vertex array object") {
  setup_opengl();
  GIVEN("an invalid vertex array object binding") {
    unsigned int shit_binding = 5;
    WHEN("a renderable is constructed with it") {
      THEN("an invalid vertex array exception is thrown") {
        //gotta lambda that motherfucker
        auto construct = [&]() {Graphics::Renderable renderable(shit_binding);};
        REQUIRE_THROWS_AS(construct(), Exceptions::InvalidVertexArrayException);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be constructed with a valid vertex array object") {
  setup_opengl();
  GIVEN("a valid vertex array object binding") {
    unsigned int good_binding;
    glGenVertexArrays(1, &good_binding); 
    glBindVertexArray(good_binding);
    glBindVertexArray(0);
    WHEN("a renderable is constructed with it") {
      Graphics::Renderable renderable(good_binding);
      THEN("the renderable is successfully created, and has the correct vertex array object handle") {
        REQUIRE(renderable.getVertexArrayBinding() == good_binding);
      }
    }
  }
  destroy_opengl();
}