#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <memory>
#include "component.h"
#include "transform.h"

class Entity {
  private:
    std::shared_ptr<Transform> transform;
  protected:
    std::list<std::shared_ptr<Component>> components;
  public:
    Entity();
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Transform> getTransform() const noexcept;
    virtual void onStart() = 0;
    virtual void onUpdate(const float delta) = 0;
    virtual void onStop() = 0;
    virtual void onDestroy() = 0;
};

#endif
