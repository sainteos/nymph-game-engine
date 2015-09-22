#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <list>
#include <memory>
#include "component.h"

class ComponentManager {
  private:
    std::list<std::shared_ptr<Component>> components;
  public:
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    void onStart();
    void onUpdate(const float delta);
    void destroy();
};
#endif
