#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <thread>
#include <atomic>
#include <map>
#include <set>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include <glm/glm.hpp>
#include "renderable.h"
#include "window_exit_functor.h"
#include "camera.h"
//= SCRIPTABLE

namespace Graphics {
  /**
   * @brief      Class for graphics system.
   */
  class [[scriptable]] GraphicsSystem {
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

      std::list<std::shared_ptr<Light>> lights;
      unsigned int max_influence_lights;

      //The next id for renderables
      int next_id;

      static void errorCallback(int error, const char* description);
      static void windowSizeCallback(GLFWwindow* window, int width, int height);

      WindowExitFunctor window_exit;

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
      void initialize(const int width, const int height, std::string name, const bool fullscreen, const WindowExitFunctor& window_exit);
      /**
       * @brief Getter to see if system is initialized
       * @details Will return true after initialize() is called.
       * @return true if initialized() has been called, false otherwise
       */
      bool isInitialized() const noexcept;

      /**
       * @brief      Determines if running.
       *
       * @return     True if running, False otherwise.
       */
      bool isRunning() noexcept;

      /**
       * @brief      Starts a render.
       */
      void startRender();
      /**
       * @brief      Stops a render.
       */
      void stopRender();
      /**
       * @brief      Starts a frame.
       */
      void startFrame();
      /**
       * @brief      Stops a frame.
       */
      void stopFrame();

      /**
       * @brief      Gets the window.
       *
       * @return     The window.
       */
      GLFWwindow* getWindow() const noexcept;

      /**
       * @brief Getter for the window width
       * @details This Pings GLFW to get the width of the window.
       * @return integer denoting pixel width of window
       */
      [[scriptable]] int windowHeight();
      /**
       * @brief Getter for the window height
       * @details This Pings GLFW to get the height of the window.
       * @return integer denoting pixel height of window
       */
      [[scriptable]] int windowWidth();

      /**
       * @brief Getter for the window name
       * @details This will return the name of the window
       * @return a string that is the name of the window
       */
      [[scriptable]] std::string windowName() const noexcept;

      /**
       * @brief      Sets the window name.
       *
       * @param[in]  name  The name
       */
      [[scriptable]] void setWindowName(const std::string& name);

      /**
       * @brief Adds a renderable to the renderable pool
       * @details Added to the pool of renderables that can be
       *          rendered during the update loop.
       *
       * @param renderable Shared Pointer to a previously created renderable
       * @return id integer of the renderable
       */
      [[scriptable]] int addRenderable(std::shared_ptr<Graphics::Renderable> renderable);
      /**
       * @brief Removes a renderable from the renderable pool
       * @details Removed from the pool of renderables to take it out of
       *          the update loop.
       *
       * @param id id integer of renderable to remove
       * @return true if successful, false if it didn't exist
       */
      [[scriptable]] bool removeRenderable(const int id);

      /**
       * @brief Returns the number of renderables currently in the system
       * @details This returns the number of renderables that have been
       *          added to the system, and are ready to be updated and rendered.
       * @return integer representing number of renderables in the system
       */
      [[scriptable]] int renderablesCount();

      /**
       * @brief      Sets the maximum number influencing lights.
       *
       * @param[in]  number  The number
       */
      [[scriptable]] void setMaxInfluenceLights(const unsigned int number) noexcept;
      /**
       * @brief      Gets the maximum number of influencing lights.
       *
       * @return     The maximum influence lights.
       */
      [[scriptable]] unsigned int getMaxInfluenceLights() const noexcept;
      /**
       * @brief      Adds a light.
       *
       * @param[in]  light  The light
       */
      [[scriptable]] void addLight(std::shared_ptr<Light> light) noexcept;
      /**
       * @brief      Removes a light.
       *
       * @param[in]  light  The light
       */
      [[scriptable]] void removeLight(std::shared_ptr<Light> light);

      GLFWwindow* getCurrentWindow() noexcept;


      /**
       * @brief Closes window, and destroys GLFW context.
       * @details This closes the window created by GLFW and terminates
       *          glfw. Initialize must be called to get another window.
       */
      void destroy();

  };
}

#endif
