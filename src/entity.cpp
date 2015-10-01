#include <easylogging++.h>
#include <typeinfo>
#include "entity.h"

Entity::Entity() {
  transform = std::make_shared<Transform>();
}

void Entity::addComponent(std::shared_ptr<Component> component) {
  component->setTransform(transform);
  components.push_back(component);
  components.unique();

  for(auto& i : components) {
    component->addObserver(i);
    i->addObserver(component);
  }
}

void Entity::removeComponent(std::shared_ptr<Component> component) {
  component->setTransform(std::make_shared<Transform>());
  components.remove(component);
  for(auto& i : components) {
    component->removeObserver(i);
    i->removeObserver(component);
  }
}

std::shared_ptr<Transform> Entity::getTransform() const noexcept {
  return transform;
}

void Entity::onStart() {
  for(auto i : components) {
    i->onStart();
  }
}

void Entity::onUpdate(const float delta) {
  for(auto i : components) {
    i->onUpdate(delta);
  }
}

void Entity::onStop() {
}

void Entity::onDestroy() {
  for(auto i : components) {
    i->onDestroy();
  }
}
