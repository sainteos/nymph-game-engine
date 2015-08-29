#include "entity.h"

void Entity::addComponent(std::shared_ptr<Component> component) {
  components.push_back(component);
}

void Entity::removeComponent(std::shared_ptr<Component> component) {
  components.remove(component);
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
