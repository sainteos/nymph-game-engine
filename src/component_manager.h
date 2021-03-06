#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <map>
#include <memory>
#include "component.h"

namespace Graphics {
  class Camera;
}
/**
 * @brief      Class for component manager.
 */
class [[scriptable]] ComponentManager {
  private:
    std::multimap<unsigned long long, std::shared_ptr<Component>> components;
    std::weak_ptr<Graphics::Camera> camera;
  public:
    /**
     * @brief      Adds a component.
     *
     * @param[in]  component  The component
     */
    [[scriptable]] void addComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Adds components.
     *
     * @param[in]  components  The components
     */
    [[scriptable]] void addComponents(std::vector<std::shared_ptr<Component>> components);
    /**
     * @brief      Adds components.
     *
     * @param[in]  components  The components
     */
    [[scriptable]] void addComponents(std::list<std::shared_ptr<Component>> components);
    /**
     * @brief      Removes a component.
     *
     * @param[in]  component  The component
     */
    [[scriptable]] void removeComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Removes components.
     *
     * @param[in]  components  The components
     */
    [[scriptable]] void removeComponents(std::vector<std::shared_ptr<Component>> components);
    /**
     * @brief      Gets the number of components
     *
     * @return     # of components
     */
    [[scriptable]] unsigned int count() const noexcept;

    /**
     * @brief      Adds a camera.
     *
     * @param[in]  component  The component
     */
    [[scriptable]] void addComponent(std::shared_ptr<Graphics::Camera> component);
    /**
     * @brief      Calls onStart on each component
     */
    void onStart();
    /**
     * @brief      Calls onUpdate on each component, once per frame
     *
     * @param[in]  delta  The delta
     */
    void onUpdate(const float delta);
    /**
     * @brief      Calls onDestroy on each component.
     */
    void destroy();

};
#endif
