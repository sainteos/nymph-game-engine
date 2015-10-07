#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "transform.h"
#include "events/subject.h"
#include "events/observer.h"

class ComponentManager;

class Component : public Events::Subject, public Events::Observer {
  protected:
    std::weak_ptr<Transform> transform;
    bool active;
    unsigned int id;

    static unsigned int next_id;

    friend class ComponentManager;
  public:
    Component();
    virtual void onStart() = 0;
    virtual const bool onUpdate(const double delta);
    virtual void onDestroy() = 0;
    void setTransform(std::shared_ptr<Transform> transform);
    std::shared_ptr<Transform> getTransform() const noexcept;

    void setActive(const bool active) noexcept;
    const bool isActive() const noexcept;

    const unsigned int getId() const noexcept;

    const bool operator<(const Component& other) const noexcept;

    virtual ~Component() {}
};
#endif
