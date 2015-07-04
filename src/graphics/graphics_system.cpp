#include <easylogging++.h>
#include "graphics_system.h"
#include <exception>
#include <stdexcept>
#include <system_error>
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"

namespace Graphics {
  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false), running(false) {
  }

  GraphicsSystem::~GraphicsSystem() {
    if(initialized)
      destroy();
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name) {
    LOG(INFO)<<"Graphics System initializing...";
    if(initialized) {
      LOG(WARNING)<<"Can't reinitialize graphics system.";
      throw Exceptions::SystemAlreadyInitializedException("Graphics");
    }

    if(!glfwInit()) {
      LOG(FATAL)<<"Glfw couldn't be initialized!";
      throw std::runtime_error("Glfw couldn't be initialized!");
    }

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    window_title = name;

    if(window == nullptr) {
      LOG(ERROR)<<"Glfw window couldn't be created.";
      throw std::runtime_error("Glfw window couldn't be created.");
    }

    glfwMakeContextCurrent(window);

    initialized = true;
    LOG(INFO)<<"Graphics system initialized!";
  }

  const bool GraphicsSystem::isInitialized() const noexcept {
    return initialized;
  }

  const int GraphicsSystem::windowWidth() {
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return width;
  }

  const int GraphicsSystem::windowHeight() {
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return height;
  }

  const std::string GraphicsSystem::windowName() const noexcept {
    return window_title;
  }

  std::thread::id GraphicsSystem::start() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    if(running.load())
      throw Exceptions::SystemAlreadyRunningException("Graphics");

    update_thread = std::thread(GraphicsSystem::updateLoop, this);
    running.store(true);
    LOG(INFO)<<"Graphics update thread started!";
    return update_thread.get_id();
  }
  
  void GraphicsSystem::stop() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    if(!running.load())
      throw Exceptions::SystemNotRunningException("Graphics");
    running.store(false);
    update_thread.join();
    LOG(INFO)<<"Graphics update thread stopped!";
  }

  const bool GraphicsSystem::isRunning() const noexcept {
    return running.load();
  }

  void GraphicsSystem::updateLoop(GraphicsSystem* instance) {
    while(instance->running.load()) {

    }
  }

  void GraphicsSystem::destroy() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
      
    if(running.load())
      stop();

    initialized = false;
    glfwTerminate();
    LOG(INFO)<<"Graphics System destroyed!";
  }
}