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
#include "exceptions/no_camera_attached_exception.h"

namespace Graphics {

  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false), next_id(1) {
  }

  GraphicsSystem::~GraphicsSystem() {
    if(initialized){
      destroy();
    }
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name, const WindowExitFunctor& window_exit) {
    LOG(INFO)<<"Graphics System initializing...";
    if(initialized) {
      LOG(WARNING)<<"Can't reinitialize graphics system.";
      throw Exceptions::SystemAlreadyInitializedException("Graphics");
    }
    
    #ifndef __APPLE__
    if (glewInit() != GLEW_OK)
    {
      LOG(ERROR)<<"Glew couldn't be initialized";
      throw std::runtime_error("Glew couldn't be initialized");
    }
    #endif

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

    glfwMakeContextCurrent(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);
    ilInit();
    
    glfwSetWindowTitle(window, window_title.c_str());

    initialized = true;
    LOG(INFO)<<"Graphics system initialized!";
  }

  const bool GraphicsSystem::isInitialized() const noexcept {
    return initialized;
  }

  const bool GraphicsSystem::isRunning() noexcept {
    return !window_exit(window);
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

  void GraphicsSystem::setWindowName(const std::string& name) {
    glfwSetWindowTitle(window, name.c_str());
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

  void GraphicsSystem::setCamera(const std::shared_ptr<Camera> camera) noexcept {
    this->camera = camera;
  }

  std::shared_ptr<Camera> GraphicsSystem::getCamera() const noexcept {
    return camera;
  }

  GLFWwindow* GraphicsSystem::getCurrentWindow() noexcept {
    return window;
  }

  void GraphicsSystem::startRender() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    glClearColor(0.0, 0.0, 0.0, 1.0);

    if(camera == nullptr)
      throw Exceptions::NoCameraAttachedException();

    camera->onStart();

    for(auto& renderables_iter : renderables_map) {
      if(renderables_iter.second->isActive()) {
        renderables_iter.second->onStart();
      }
    }
  }

  void GraphicsSystem::renderFrame(const float delta) {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->onUpdate(delta);

    renderables_mutex.lock();
    for(auto& renderables_iter : renderables_map) {
      if(renderables_iter.second->isActive() && camera->isRenderableWithin(renderables_iter.second) && renderables_iter.second->onUpdate(delta)) {
        //if renderable was updated, then render
        renderables_iter.second->onRender();
      }
    }
    renderables_mutex.unlock();

    glfwSwapBuffers(window);
  }

  GLFWwindow* GraphicsSystem::getWindow() const noexcept {
    return window;
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