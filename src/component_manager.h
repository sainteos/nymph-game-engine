#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <map>
#include <memory>
#include <map>
#include "component.h"
#include "graphics/camera.h"

//= SCRIPTABLE

class ComponentManager {
  private:
    std::multimap<unsigned long long, std::shared_ptr<Component>> components;
    std::weak_ptr<Graphics::Camera> camera;
  public:
    //= BEGIN SCRIPTABLE
    void addComponent(std::shared_ptr<Component> component);
    void addComponents(std::vector<std::shared_ptr<Component>> components);
    void addComponents(std::list<std::shared_ptr<Component>> components);
    void removeComponent(std::shared_ptr<Component> component);
    void removeComponents(std::vector<std::shared_ptr<Component>> components);
    const unsigned int count() const noexcept;
    //= END SCRIPTABLE
    void addComponent(std::shared_ptr<Graphics::Camera> component);
    void onStart();
    void onUpdate(const float delta);
    void destroy();

};
#endif
