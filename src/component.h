#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "transform.h"
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"

class ComponentManager;
class Entity;

class Component : public Events::Subject, public Events::Observer {
  protected:
    std::weak_ptr<Entity> entity;
    std::shared_ptr<Transform> transform;
    bool active;
    unsigned int id;

    static unsigned int next_id;

    friend class ComponentManager;
    friend class Entity;
  public:
    Component();
    virtual void onStart() = 0;
    virtual const bool onUpdate(const double delta) = 0;
    virtual void onDestroy() = 0;
    void setTransform(std::shared_ptr<Transform> transform);
    std::shared_ptr<Transform> getTransform() const noexcept;

    void setActive(const bool active) noexcept;
    const bool isActive() const noexcept;


    const unsigned int getId() const noexcept;
    virtual const unsigned long long getValueForSorting() const = 0;

    const bool operator<(Component& other) noexcept;

    virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
    virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

    virtual ~Component() {}
};
#endif
