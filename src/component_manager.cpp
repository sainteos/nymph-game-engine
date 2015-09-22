#include "component_manager.h"


void ComponentManager::addComponent(std::shared_ptr<Component> component) {
  components.push_back(component);
}

void ComponentManager::removeComponent(std::shared_ptr<Component> component) {
  components.remove(component);
}

void ComponentManager::onStart() {
  for(auto component : components) {
    component->onStart();
  }
}

void ComponentManager::onUpdate(const float delta) {
  for(auto component : components) {
    if(component->isActive()) {
      component->onUpdate(delta);
    }
  }
}

void ComponentManager::destroy() {
  for(auto component : components) {
    component->onDestroy();
    component.reset();
  }
  components.clear();
}
