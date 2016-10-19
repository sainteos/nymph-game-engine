#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/camera.h"

//= SCRIPTABLE

class Entity  : public std::enable_shared_from_this<Entity>  {
  private:
    std::shared_ptr<Transform> transform;
    bool active;
  protected:
    std::list<std::shared_ptr<Component>> components;
  public:
    //= BEGIN SCRIPTABLE
    Entity();
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    std::list<std::shared_ptr<Component>>& getComponents() noexcept;
    std::shared_ptr<Transform> getTransform() const noexcept;
    void setActive(const bool active) noexcept;
    const bool isActive() const noexcept;
    //= END SCRIPTABLE
};

#endif
