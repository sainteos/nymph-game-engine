#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <map>
#include <memory>
#include "component.h"
#include "graphics/camera.h"

class ComponentManager {
  private:
    std::multimap<unsigned long long, std::shared_ptr<Component>> components;
    std::weak_ptr<Graphics::Camera> camera;
  public:
    void addComponent(std::shared_ptr<Component> component);
    void addComponent(std::shared_ptr<Graphics::Camera> component);
    void removeComponent(std::shared_ptr<Component> component);
    void onStart();
    void onUpdate(const float delta);
    void destroy();
};
#endif
