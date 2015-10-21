#include "component_manager.h"
#include "exceptions/no_camera_attached_exception.h"
#include <easylogging++.h>
#include <typeinfo>


void ComponentManager::addComponent(std::shared_ptr<Component> component) {
  components.insert(component);
}

void ComponentManager::addComponent(std::shared_ptr<Graphics::Camera> component) {
  components.insert(component);
  camera = component;
}

void ComponentManager::removeComponent(std::shared_ptr<Component> component) {
  components.erase(component);
}

void ComponentManager::onStart() {
  if(camera.lock() == nullptr)
    throw Exceptions::NoCameraAttachedException();
  for(auto component : components) {
    component->onStart();
  }
}

void ComponentManager::onUpdate(const float delta) {
  if(camera.lock() == nullptr)
    throw Exceptions::NoCameraAttachedException();
  for(auto component : components) {
    //if(camera.lock()->isComponentWithin(*component)) {
      component->onUpdate(delta);
    //}
  }
}

void ComponentManager::destroy() {
  for(auto component : components) {
    component->onDestroy();
    component.reset();
  }
  components.clear();
}
