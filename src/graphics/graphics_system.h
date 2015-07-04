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
      /**
       * @brief Initializes the graphics system.
       * @details This function initializes the graphics system by
       * creating a GLFW based window at the specified size, and having
       * the supplied name as the window title.
       * 
       * @param width window width in px
       * @param height window height in px
       * @param name string containing the name of the window to be built
       */
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
