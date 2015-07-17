#include <easylogging++.h>
#include "graphics_system.h"
#include <exception>
#include <stdexcept>
#include <system_error>
#include <chrono>
#include <string>
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"

namespace Graphics {

  using clock = std::chrono::high_resolution_clock;
  using microseconds = std::chrono::microseconds;

  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false), running(false), next_id(1) {
  }

  GraphicsSystem::~GraphicsSystem() {
    if(initialized)
      destroy();
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name, const double max_fps) {
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
    
    this->max_fps = max_fps;

    glfwMakeContextCurrent(window);

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

  std::thread::id GraphicsSystem::start() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    if(running.load())
      throw Exceptions::SystemAlreadyRunningException("Graphics");

    update_thread = std::thread(GraphicsSystem::updateLoop, this);
    running.store(true);

    last_time = clock::now();

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

  void GraphicsSystem::updateLoop(GraphicsSystem* instance) {
    auto current_time = clock::now();
    auto delta = std::chrono::duration_cast<microseconds>(current_time - instance->last_time).count() / 1000.0;
    while(instance->running.load()) {

      instance->renderables_mutex.lock();
      for(auto renderables_iter : instance->renderables_map) {
        if(renderables_iter.second->isActive() && renderables_iter.second->onUpdate(delta)) {
          //if renderable was updated, then render
          renderables_iter.second->onRender();
        }
      }
      instance->renderables_mutex.unlock();

      //if we have a max fps
      if(instance->max_fps > 0.0) {
        //keep sleeping until we hit the right time
        while(std::chrono::duration_cast<microseconds>(clock::now() - instance->last_time).count() / 1000.0 < 1.0 / (instance->max_fps / 1000.0)) {
          std::this_thread::sleep_for(microseconds(1));
        }
      }

      //update the current fps
      auto now_time = clock::now();
      instance->current_fps.store(1.0 / (std::chrono::duration_cast<microseconds>(now_time - instance->last_time).count() / 1000000.0));
      //set last time to the now time
      instance->last_time = now_time;
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