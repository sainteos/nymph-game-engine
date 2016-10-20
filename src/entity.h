#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/camera.h"

//= SCRIPTABLE

/**
 * @brief      Class for entity in the engine.
 */
class Entity  : public std::enable_shared_from_this<Entity>  {
  private:
    std::shared_ptr<Transform> transform;
    bool active;
  protected:
    std::list<std::shared_ptr<Component>> components;
  public:
    //= BEGIN SCRIPTABLE
    
    /**
     * @brief      Entity constructor
     */
    Entity();
    /**
     * @brief      Adds a component.
     *
     * @param[in]  component  The component
     */
    void addComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Removes a component.
     *
     * @param[in]  component  The component
     */
    void removeComponent(std::shared_ptr<Component> component);
    /**
     * @brief      Gets the components.
     *
     * @return     The components.
     */
    std::list<std::shared_ptr<Component>>& getComponents() noexcept;
    /**
     * @brief      Gets the transform.
     *
     * @return     The transform.
     */
    std::shared_ptr<Transform> getTransform() const noexcept;
    /**
     * @brief      Sets entity active or inactive.
     *
     * @param[in]  active  True if active
     */
    void setActive(const bool active) noexcept;
    /**
     * @brief      Determines if active.
     *
     * @return     True if active, False otherwise.
     */
    const bool isActive() const noexcept;
    //= END SCRIPTABLE
};

#endif
