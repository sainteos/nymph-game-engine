#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <thread>
#include <atomic>
#include <map>
#include <set>
#include <mutex>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include <glm/glm.hpp>
#include "graphics/renderable.h"
#include "graphics/window_exit_functor.h"
#include "graphics/camera.h"

namespace Graphics {
  class GraphicsSystem {
    private:
      struct RankedLight {
        std::shared_ptr<Light> light;
        float influence;
        friend bool operator<(const RankedLight& left, const RankedLight& right) {
          return left.influence < right.influence;
        }
      };

      GLFWwindow* window;
      bool initialized;
      float delta;
      float delta_accum;

      std::string window_title;

      std::map<int, std::shared_ptr<Graphics::Renderable>> renderables_map;
      std::mutex renderables_mutex;

      std::list<std::shared_ptr<Light>> lights;
      unsigned int max_influence_lights;

      //The next id for renderables
      int next_id;
      
      static void errorCallback(int error, const char* description);

      WindowExitFunctor window_exit;
      
      std::shared_ptr<Camera> camera;

    public:
      GraphicsSystem();
      ~GraphicsSystem();
      /**
       * @brief Initializes the graphics system.
       * @details This function initializes the graphics system by
       * creating a GLFW based window at the specified size, and having
       * the supplied name as the window title. Max FPS can also be
       * supplied to allow VSYNC.
       * 
       * @param width window width in px
       * @param height window height in px
       * @param name string containing the name of the window to be built
       * @param max_fps double containing the max allowed fps. Default 0.0.
       */
      void initialize(const int width, const int height, std::string name, const WindowExitFunctor& window_exit);
      /**
       * @brief Getter to see if system is initialized
       * @details Will return true after initialize() is called.
       * @return true if initialized() has been called, false otherwise
       */
      const bool isInitialized() const noexcept;

      const bool isRunning() noexcept;

      void startRender();
      void renderFrame(const float delta);
      void stopRender();

      GLFWwindow* getWindow() const noexcept;
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

      void setWindowName(const std::string& name);
      
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
       * @brief Removes a renderable from the renderable pool
       * @details Removed from the pool of renderables to take it out of
       *          the update loop.
       * 
       * @param id id integer of renderable to remove
       * @return true if successful, false if it didn't exist
       */
      const bool removeRenderable(const int id);

      /**
       * @brief Returns the number of renderables currently in the system
       * @details This returns the number of renderables that have been 
       *          added to the system, and are ready to be updated and rendered.
       * @return integer representing number of renderables in the system
       */
      const int renderablesCount();

      void setCamera(const std::shared_ptr<Camera> camera) noexcept;
      std::shared_ptr<Camera> getCamera() const noexcept;

      GLFWwindow* getCurrentWindow() noexcept;

      void setMaxInfluenceLights(const unsigned int number) noexcept;
      const unsigned int getMaxInfluenceLights() const noexcept;
      void addLight(std::shared_ptr<Light> light) noexcept;
      void removeLight(std::shared_ptr<Light> light);

      /**
       * @brief Closes window, and destroys GLFW context.
       * @details This closes the window created by GLFW and terminates 
       *          glfw. Initialize must be called to get another window.
       */
      void destroy();
  };
}

#endif
