#include <easylogging++.h>
#include "graphics_system.h"
#include <exception>
#include <stdexcept>
#include <system_error>
#include <chrono>
#include <string>
#include <sstream>
#include <IL/il.h>
#include <glm/gtc/matrix_transform.hpp>
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"

namespace Graphics {

  using clock = std::chrono::high_resolution_clock;
  using microseconds = std::chrono::microseconds;

  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false), next_id(1), projection_matrix(1.0) {
  }

  GraphicsSystem::~GraphicsSystem() {
    if(initialized){
      destroy();
    }
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name, const WindowExitFunctor& window_exit, const double max_fps) {
    LOG(INFO)<<"Graphics System initializing...";
    if(initialized) {
      LOG(WARNING)<<"Can't reinitialize graphics system.";
      throw Exceptions::SystemAlreadyInitializedException("Graphics");
    }

    glfwSetErrorCallback(errorCallback);


    if(!glfwInit()) {
      LOG(FATAL)<<"Glfw couldn't be initialized!";
      throw std::runtime_error("Glfw couldn't be initialized!");
    }
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    window_title = name;
    this->window_exit = window_exit;
    if(window == nullptr) {
      LOG(ERROR)<<"Glfw window couldn't be created.";
      throw std::runtime_error("Glfw window couldn't be created.");
    }
    
    this->max_fps = max_fps;

    glfwMakeContextCurrent(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glClearDepth(1.0f);
    ilInit();
    
    float tile_width = 32.0;
    float tile_height = 32.0;

    float width_in_tiles = 21.0;
    float height_in_tiles = 12.0;

    projection_matrix = glm::ortho(tile_width * width_in_tiles / -2.0f, tile_width * width_in_tiles / 2.0f, tile_height * height_in_tiles / -2.0f, tile_height * height_in_tiles / 2.0f, 0.1f, 40.0f);

    initialized = true;
    LOG(INFO)<<"Graphics system initialized!";
  }

  const bool GraphicsSystem::isInitialized() const noexcept {
    return initialized;
  }

  const int GraphicsSystem::windowWidth() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return width;
  }

  const int GraphicsSystem::windowHeight() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return height;
  }

  const std::string GraphicsSystem::windowName() const noexcept {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    return window_title;
  }

  const int GraphicsSystem::addRenderable(std::shared_ptr<Graphics::Renderable> renderable) {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    std::lock_guard<std::mutex> lock(renderables_mutex);
    renderables_map[next_id] = renderable;
    return next_id++;
  }

  const bool GraphicsSystem::removeRenderable(const int id) {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    if(id < 1) 
      throw std::out_of_range("Cannot have id less than 1");

    std::lock_guard<std::mutex> lock(renderables_mutex);
    if(renderables_map.count(id) <= 0)
      return false;
    renderables_map.erase(id);
    return true;
  }

  const int GraphicsSystem::renderablesCount() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    std::lock_guard<std::mutex> lock(renderables_mutex);
    return renderables_map.size();
  }

  const double GraphicsSystem::getMaxFPS() const noexcept {
    return max_fps;
  }

  const double GraphicsSystem::getCurrentFPS() const noexcept {
    return current_fps.load();
  }

  GLFWwindow* GraphicsSystem::getCurrentWindow() noexcept {
    return window;
  }

  void GraphicsSystem::renderLoop() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    LOG(INFO)<<"Render loop started.";
    bool running = true;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time = clock::now();
    auto current_time = clock::now();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    for(auto& renderables_iter : renderables_map) {
      if(renderables_iter.second->isActive()) {
        renderables_iter.second->onStart();
        
        //this probably needs to be moved elsewhere
        if(renderables_iter.second->getShader())
          renderables_iter.second->getShader()->setUniform("projection_matrix", projection_matrix);
      }
    }
    auto delta = std::chrono::duration_cast<microseconds>(current_time - last_time).count() / 1000.0;

    while(running) {
      std::stringstream title;
      title << window_title << "      FPS: " << getCurrentFPS();
      glfwSetWindowTitle(window, title.str().c_str());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      renderables_mutex.lock();
      for(auto& renderables_iter : renderables_map) {
        if(renderables_iter.second->isActive() && renderables_iter.second->onUpdate(delta)) {
          //if renderable was updated, then render
          renderables_iter.second->onRender();
        }
      }
      renderables_mutex.unlock();

      //if we have a max fps
      if(max_fps > 0.0) {
        //keep sleeping until we hit the right time
        while(std::chrono::duration_cast<microseconds>(clock::now() - last_time).count() / 1000.0 < 1.0 / (max_fps / 1000.0)) {
          std::this_thread::sleep_for(microseconds(1));
        }
      }
      glfwSwapBuffers(window);
      //update the current fps
      current_time = clock::now();
      current_fps.store(1.0 / (std::chrono::duration_cast<microseconds>(current_time - last_time).count() / 1000000.0));
      //set last time to the now time
      delta = std::chrono::duration_cast<microseconds>(current_time - last_time).count() / 1000.0;
      last_time = current_time;
      running = !window_exit(window);
    }
  }

  void GraphicsSystem::errorCallback(int error, const char* description) {
    switch(error) {
      case GLFW_NOT_INITIALIZED:
        LOG(ERROR)<<description;
        break;
      default:
        LOG(ERROR)<<"Unknown error: "<<description;
        break;
    }
  }

  void GraphicsSystem::destroy() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    initialized = false;
    glfwTerminate();
    LOG(INFO)<<"Graphics System destroyed!";
  }
}