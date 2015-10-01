#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "transform.h"
#include "events/subject.h"
#include "events/observer.h"

class Component : public Events::Subject, public Events::Observer {
  protected:
    std::shared_ptr<Transform> transform;
    bool active;
    unsigned int id;

    static unsigned int next_id;
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

    virtual void onNotify(const Events::Event& event) override;

    virtual ~Component() {}
};
#endif