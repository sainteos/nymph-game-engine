#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <vector>
#include "transform.h"
#include "component.h"
#include "entity.h"

class Scene {
  private:
    std::shared_ptr<Transform> transform;
    std::vector <std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Entity>> entities;
    std::string name;

  public:
    Scene();  
    void setTransform(std::shared_ptr<Transform> transform) noexcept;
    std::shared_ptr<Transform> getTransform() const noexcept;
    void addComponent(std::shared_ptr<Component> component);
    void addComponents(std::vector<std::shared_ptr<Component>> components);
    std::vector<std::shared_ptr<Component>> getComponents() const noexcept;
    void addEntity(std::shared_ptr<Entity> entity) noexcept;
    void addEntities(std::vector<std::shared_ptr<Entity>> entities) noexcept;
    std::vector<std::shared_ptr<Entity>> getEntities() const noexcept;
    void setName(const std::string name) noexcept;
    const std::string getName() const noexcept;
};

#endif
