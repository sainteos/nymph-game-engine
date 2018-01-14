#include <easylogging++.h>
#include "graphics_system.h"
#include <exception>
#include <stdexcept>
#include <system_error>
#include <chrono>
#include <string>
#include <sstream>
#include <queue>
#include <IL/il.h>
#include <glm/gtc/matrix_transform.hpp>
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>

namespace Graphics {

  GraphicsSystem::GraphicsSystem() : window(nullptr), initialized(false), next_id(1), max_influence_lights(8) {
  }

  GraphicsSystem::~GraphicsSystem() {
    if(initialized){
      destroy();
    }
  }

  void GraphicsSystem::initialize(const int width, const int height, const std::string name, const bool fullscreen, const WindowExitFunctor& window_exit) {
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWmonitor* monitor = nullptr;
    if(fullscreen) {
      monitor = glfwGetPrimaryMonitor();
    }
    window = glfwCreateWindow(width, height, name.c_str(), monitor, nullptr);
    window_title = name;
    this->window_exit = window_exit;
    if(window == nullptr) {
      LOG(ERROR)<<"Glfw window couldn't be created.";
      throw std::runtime_error("Glfw window couldn't be created.");
    }

    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      if(!gladLoadGL()) {
        LOG(ERROR)<<"Glad could not be initialized!";
        throw std::runtime_error("Glad could not be initialized!");  
      }
    #endif

    ilInit();
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);

    glfwSetWindowTitle(window, window_title.c_str());

    initialized = true;
    LOG(INFO)<<"Graphics system initialized!";
  }

  bool GraphicsSystem::isInitialized() const noexcept {
    return initialized;
  }

  bool GraphicsSystem::isRunning() noexcept {
    return !window_exit(window);
  }

  int GraphicsSystem::windowWidth() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return width;
  }

  int GraphicsSystem::windowHeight() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    return height;
  }

  std::string GraphicsSystem::windowName() const noexcept {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    return window_title;
  }

  void GraphicsSystem::setWindowName(const std::string& name) {
    glfwSetWindowTitle(window, name.c_str());
  }

  int GraphicsSystem::addRenderable(std::shared_ptr<Graphics::Renderable> renderable) {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    renderables_map[next_id] = renderable;
    return next_id++;
  }

  bool GraphicsSystem::removeRenderable(const int id) {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    if(id < 1)
      throw std::out_of_range("Cannot have id less than 1");

    if(renderables_map.count(id) <= 0)
      return false;
    renderables_map.erase(id);
    return true;
  }

  int GraphicsSystem::renderablesCount() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    return renderables_map.size();
  }

  GLFWwindow* GraphicsSystem::getCurrentWindow() noexcept {
    return window;
  }

  void GraphicsSystem::startRender() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);
  }

  void GraphicsSystem::startFrame() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // for(auto& renderables_iter : renderables_map) {
    //   if(renderables_iter.second->isActive() && !camera->isRenderableWithin(renderables_iter.second)) {
    //     renderables_iter.second->setActive(false);
    //   }
    //   else if(!renderables_iter.second->isActive() && camera->isRenderableWithin(renderables_iter.second)) {
    //     renderables_iter.second->setActive(true);
    //   }
      //   if(renderables_iter.second->isLightReactive()) {
      //     //find lights that influence renderable
      //     std::priority_queue<RankedLight, std::vector<RankedLight>, std::less<RankedLight>> light_queue;

      //     for(auto& light : lights) {
      //       auto influence = light->influenceOnComponent(*renderables_iter.second);
      //       if(influence > 0.0)
      //         light_queue.push(RankedLight { light,  influence });
      //     }
      //     renderables_iter.second->clearInfluencingLights();
      //     for(int i = 0; i < max_influence_lights && !light_queue.empty(); i++) {
      //       renderables_iter.second->addInfluencingLight(light_queue.top().light);
      //       light_queue.pop();
      //     }
      //   }
      // }
    // }
  }

  void GraphicsSystem::stopFrame() {
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

  void GraphicsSystem::windowSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void GraphicsSystem::setMaxInfluenceLights(const unsigned int number) noexcept {
    max_influence_lights = number;
  }

  unsigned int GraphicsSystem::getMaxInfluenceLights() const noexcept {
    return max_influence_lights;
  }

  void GraphicsSystem::addLight(std::shared_ptr<Light> light) noexcept {
    lights.push_back(light);
  }

  void GraphicsSystem::removeLight(std::shared_ptr<Light> light) {
    lights.remove(light);
  }

  void GraphicsSystem::destroy() {
    if(!initialized)
      throw Exceptions::SystemNotInitializedException("Graphics");

    initialized = false;
    glfwTerminate();
    LOG(INFO)<<"Graphics System destroyed!";
  }

}
