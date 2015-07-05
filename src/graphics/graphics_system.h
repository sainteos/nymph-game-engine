#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <thread>
#include <atomic>
#include <map>
#include <glfw3.h>
#include "graphics/renderable.h"

namespace Graphics {
  class GraphicsSystem {
    private:
      GLFWwindow* window;
      bool initialized;
      std::atomic_bool running;
      std::string window_title;
      std::thread update_thread;

      std::map<int, std::shared_ptr<Graphics::Renderable>> renderables_map;
      //The next id for renderables
      int next_id;
      
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
      /**
       * @brief Getter to see if system is initialized
       * @details Will return true after initialize() is called.
       * @return true if initialized() has been called, false otherwise
       */
      const bool isInitialized() const noexcept;
      /**
       * @brief Getter for the window width
       * @details This Pings GLFW to get the width of the window.
       * @return integer denoting pixel width of window
       */
      const int windowHeight();
      /**
       * @brief Getter for the window height
       * @details This Pings GLFW to get the height of the window.
       * @return integer denoting pixel height of window
       */
      const int windowWidth();

      /**
       * @brief Getter for the window name
       * @details This will return the name of the window
       * @return a string that is the name of the window
       */
      const std::string windowName() const noexcept;
      /**
       * @brief starts the update loop
       * @details Creates a seperate thread, and starts the update loop in it
       * @return thread id of the thread created
       */
      std::thread::id start();
      /**
       * @brief stops the update loop
       * @details stops the update loop and joins the extra thread
       */
      void stop();

      /**
       * @brief Getter to see if the update loop is running
       * @details Returns whether or not the update loop is running
       * @return True if update loop is running, false otherwise
       */
      const bool isRunning() const noexcept;
      
      /**
       * @brief Adds a renderable to the renderable pool 
       * @details Added to the pool of renderables that can be
       *          rendered during the update loop.
       * 
       * @param renderable Shared Pointer to a previously created renderable
       * @return id integer of the renderable
       */
      const int addRenderable(std::shared_ptr<Graphics::Renderable> renderable);
      /**
       * @brief Remves a renderable from the renderable pool
       * @details Removed from the pool of renderables to take it out of
       *          the update loop.
       * 
       * @param id id integer of renderable to remove
       * @return true if successful, false if it didn't exist
       */
      const bool removeRenderable(const int id);

      const int renderablesCount() const noexcept;

      /**
       * @brief Closes window, and destroys GLFW context.
       * @details This closes the window created by GLFW and terminates 
       *          glfw. Initialize must be called to get another window.
       */
      void destroy();
  };
}

#endif
