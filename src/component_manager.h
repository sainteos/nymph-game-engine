#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <map>
#include <memory>
#include <map>
#include "component.h"
#include "graphics/camera.h"

//= SCRIPTABLE

/**
 * @brief      Class for component manager.
 */
class ComponentManager {
  private:
    std::multimap<unsigned long long, std::shared_ptr<Component>> components;
    std::weak_ptr<Graphics::Camera> camera;
  public:
    //= BEGIN SCRIPTABLE
    
    /**
     * @brief      Adds a component.
     *
     * @param[in]  component  The component
     */
    void addComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Adds components.
     *
     * @param[in]  components  The components
     */
    void addComponents(std::vector<std::shared_ptr<Component>> components);
    /**
     * @brief      Adds components.
     *
     * @param[in]  components  The components
     */
    void addComponents(std::list<std::shared_ptr<Component>> components);
    /**
     * @brief      Removes a component.
     *
     * @param[in]  component  The component
     */
    void removeComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Removes components.
     *
     * @param[in]  components  The components
     */
    void removeComponents(std::vector<std::shared_ptr<Component>> components);
    /**
     * @brief      Gets the number of components
     *
     * @return     # of components
     */
    const unsigned int count() const noexcept;
    //= END SCRIPTABLE
    
    /**
     * @brief      Adds a camera.
     *
     * @param[in]  component  The component
     */
    void addComponent(std::shared_ptr<Graphics::Camera> component);
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
