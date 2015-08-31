#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <memory>
#include "component.h"
#include "transform.h"

class Entity {
  private:
    std::list<std::shared_ptr<Component>> components;
    std::shared_ptr<Transform> transform;
  public:
    Entity();
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Transform> getTransform() const noexcept;
    virtual void onStart();
    virtual void onUpdate(const float delta);
    virtual void onStop();
    virtual void onDestroy();
};

#endif
