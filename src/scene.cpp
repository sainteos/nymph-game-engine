#include <easylogging++.h>
#include "scene.h"


Scene::Scene()   {
  this->transform = std::make_shared<Transform>();
}

void Scene::setTransform(std::shared_ptr<Transform> transform) noexcept {
  this->transform = transform;
}

std::shared_ptr<Transform> Scene::getTransform() const noexcept {
  return this->transform;
}

void Scene::addComponent(std::shared_ptr<Component> component) {
  this->components.push_back(component);
}

void Scene::addComponents(std::vector<std::shared_ptr<Component>> components) {
  this->components.insert(this->components.end(), components.begin(), components.end());
}

std::vector<std::shared_ptr<Component>> Scene::getComponents() const noexcept {
  return this->components;
}

void Scene::addEntity(std::shared_ptr<Entity> entity) noexcept {
  this->entities.push_back(entity);
  for(auto component : entity->getComponents()) {
    this->transform->addChild(component->getTransform());
    this->components.push_back(component);
  }
}

void Scene::addEntities(std::vector<std::shared_ptr<Entity>> entities) noexcept {
  this->entities.insert(this->entities.end(), entities.begin(), entities.end());
  for(auto entity : entities) {
    for(auto component : entity->getComponents()) {
      this->transform->addChild(component->getTransform());
      this->components.push_back(component);
    }
  }
}

std::vector<std::shared_ptr<Entity>> Scene::getEntities() const noexcept {
  return this->entities;
}

void Scene::setName(const std::string name) noexcept {
  this->name = name;
}

const std::string Scene::getName() const noexcept {
  return this->name;
}
