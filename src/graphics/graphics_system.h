#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <memory>
#include <glfw3.h>

namespace Graphics {
  class GraphicsSystem {
    private:
      GLFWwindow* window;
      bool initialized;
      std::string window_title;

    public:
      GraphicsSystem();
      ~GraphicsSystem();

      void initialize(const int width, const int height, std::string name);
      const bool isInitialized() const noexcept;

      const int windowHeight();
      const int windowWidth();

      const std::string windowName() const noexcept;

      void destroy();
  };
}

#endif
