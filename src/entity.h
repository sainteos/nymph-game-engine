#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <memory>
#include "component.h"

class Entity {
  private:
    std::list<std::shared_ptr<Component>> components;
  public:
    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);
    virtual void onStart();
    virtual void onUpdate(const float delta);
    virtual void onStop();
    virtual void onDestroy();
};

#endif
