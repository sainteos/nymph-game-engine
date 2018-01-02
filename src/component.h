#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <chaiscript/chaiscript.hpp>
#include "transform.h"
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Subject Observer

class ComponentManager;
class Entity;

/**
 * @brief      Base Class for all components.
 */
class Component : public Events::Subject, public Events::Observer, virtual public el::Loggable  {
  protected:
    std::weak_ptr<Entity> entity;
    std::shared_ptr<Transform> transform;
    bool active;
    unsigned int id;

    static unsigned int next_id;

    friend class ComponentManager;
    friend class Entity;
  public:
    /**
     * @brief      Component constructor
     */
    Component();
    /**
     * @brief      Called when the engine starts and when a new scene is loaded
     */
    virtual void onStart() = 0;
    /**
     * @brief      Called every engine loop
     *
     * @param[in]  delta  The delta
     *
     * @return     true if active
     */
    virtual bool onUpdate(const double delta) = 0;
    /**
     * @brief      Called when the engine is shutting down
     */
    virtual void onDestroy() = 0;

    //= BEGIN SCRIPTABLE

    /**
     * @brief      Sets the transform.
     *
     * @param[in]  transform  The transform
     */
    void setTransform(std::shared_ptr<Transform> transform);
    /**
     * @brief      Gets the transform.
     *
     * @return     The transform.
     */
    std::shared_ptr<Transform> getTransform() const noexcept;

    /**
     * @brief      Sets the component active or inactive.
     *
     * @param[in]  active  True if active
     */
    void setActive(const bool active) noexcept;
    /**
     * @brief      Determines if active.
     *
     * @return     True if active, False otherwise.
     */
    bool isActive() const noexcept;

    /**
     * @brief      Gets the identifier.
     *
     * @return     The identifier.
     */
    unsigned int getId() const noexcept;
    /**
     * @brief      Returns a string representation of the object.
     *
     * @return     String representation of the object.
     */
    virtual std::string to_string() const noexcept;
    /**
     * @brief      Returns a string representing the class name
     *
     * @return     The class name
     */
    virtual std::string className() const noexcept;
    //= END SCRIPTABLE

    /**
     * @brief      Gets the value for sorting.
     *
     * @return     The value for sorting.
     */
    virtual unsigned long long getValueForSorting() const = 0;

    /**
     * @brief      Operator < using getValueForSorting
     *
     * @param      other  The other
     *
     * @return     true if this shoule be sorted before other
     */
    bool operator<(Component& other) noexcept;

    virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
    virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

    virtual void log(el::base::type::ostream_t& os) const override;

    /**
     * @brief      Destroys the object.
     */
    virtual ~Component() {}
};
#endif
