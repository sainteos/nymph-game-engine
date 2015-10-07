#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/camera.h"

class Entity {
  private:
    std::shared_ptr<Transform> transform;
    bool active;
  protected:
    std::list<std::shared_ptr<Component>> components;
  public:
    Entity();
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    std::list<std::shared_ptr<Component>> getComponents() const noexcept;
    std::shared_ptr<Transform> getTransform() const noexcept;
    void setActive(const bool active) noexcept;
    const bool isActive() const noexcept;
};

#endif
