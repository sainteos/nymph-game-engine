#include <easylogging++.h>
#include <typeinfo>
#include "entity.h"

Entity::Entity() : active(true) {
  transform = std::make_shared<Transform>();
}

void Entity::addComponent(std::shared_ptr<Component> component) {
  if(component->getTransform()->getParent() == nullptr) {
    transform->addChild(component->getTransform());
  }
  component->entity = shared_from_this();
  components.push_back(component);
  components.unique();

  for(auto& i : components) {
    component->addObserver(i);
    i->addObserver(component);
  }
}

void Entity::removeComponent(std::shared_ptr<Component> component) {
  transform->removeChild(component->getTransform());
  components.remove(component);
  component->entity.reset();
  for(auto& i : components) {
    component->removeObserver(i);
    i->removeObserver(component);
  }
}

std::list<std::shared_ptr<Component>>& Entity::getComponents() noexcept {
  return components;
}

std::shared_ptr<Transform> Entity::getTransform() const noexcept {
  return transform;
}

void Entity::setActive(const bool active) noexcept {
  this->active = active;
  for(auto& i : components) {
    i->setActive(active);
  }
}

const bool Entity::isActive() const noexcept {
  return active;
}