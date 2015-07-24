#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "opengl_setup.h"


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