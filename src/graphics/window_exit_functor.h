#ifndef WINDOW_EXIT_FUNCTOR_H
#define WINDOW_EXIT_FUNCTOR_H
#include <glfw3.h>

namespace Graphics {
  class WindowExitFunctor {
    public:
      bool operator()(GLFWwindow* window) {
        //TODO Move poll events to a more logical place
        glfwPollEvents();
        return (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) || glfwWindowShouldClose(window);
      }
  };
}

#endif
