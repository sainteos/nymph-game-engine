#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/camera.h"

/**
 * @brief      Class for entity in the engine.
 */
class [[scriptable]] Entity  : public std::enable_shared_from_this<Entity>  {
  private:
    std::shared_ptr<Transform> transform;
    bool active;
  protected:
    std::list<std::shared_ptr<Component>> components;
  public:
    /**
     * @brief      Entity constructor
     */
    [[scriptable]] Entity();
    /**
     * @brief      Adds a component.
     *
     * @param[in]  component  The component
     */
    [[scriptable]] void addComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Removes a component.
     *
     * @param[in]  component  The component
     */
    [[scriptable]] void removeComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Gets the components.
     *
     * @return     The components.
     */
    [[scriptable]] std::list<std::shared_ptr<Component>>& getComponents() noexcept;
    /**
     * @brief      Gets the transform.
     *
     * @return     The transform.
     */
    [[scriptable]] std::shared_ptr<Transform> getTransform() const noexcept;
    /**
     * @brief      Sets entity active or inactive.
     *
     * @param[in]  active  True if active
     */
    [[scriptable]] void setActive(const bool active) noexcept;
    /**
     * @brief      Determines if active.
     *
     * @return     True if active, False otherwise.
     */
    [[scriptable]] bool isActive() const noexcept;
};

#endif
