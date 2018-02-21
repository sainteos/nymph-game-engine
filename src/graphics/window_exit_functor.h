#ifndef WINDOW_EXIT_FUNCTOR_H
#define WINDOW_EXIT_FUNCTOR_H
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>

namespace Graphics {
  class WindowExitFunctor {
    public:
      bool operator()(GLFWwindow* window) {
        return (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || glfwWindowShouldClose(window);
      }
  };
}

#endif
