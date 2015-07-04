#include <easylogging++.h>
#include "graphics_system.h"
#include <exception>
#include <stdexcept>
#include <system_error>
#include "exceptions/system_already_initialized_exception.h"

namespace Graphics {
  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false) {
  }

  GraphicsSystem::~GraphicsSystem() {
    
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name) {
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

  const std::string GraphicsSystem::windowName() const noexcept{
    return window_title;
  }

  void GraphicsSystem::destroy() {
    glfwTerminate();
  }
}