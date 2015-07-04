#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <thread>
#include <atomic>
#include <glfw3.h>

namespace Graphics {
  class GraphicsSystem {
    private:
      GLFWwindow* window;
      bool initialized;
      std::atomic_bool running;
      std::string window_title;
      std::thread update_thread;

      static void updateLoop(GraphicsSystem* instance);

    public:
      GraphicsSystem();
      ~GraphicsSystem();

      void initialize(const int width, const int height, std::string name);
      const bool isInitialized() const noexcept;

      const int windowHeight();
      const int windowWidth();

      const std::string windowName() const noexcept;

      std::thread::id start();
      void stop();

      const bool isRunning() const noexcept;

      void destroy();
  };
}

#endif
