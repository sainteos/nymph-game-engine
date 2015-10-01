#ifndef COMPONENT_TRIGGER_EVENT_H
#define COMPONENT_TRIGGER_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

template<typename T>
class ComponentTriggerEvent : public Events::Event {
  private:
    T state;
  public:
    ComponentTriggerEvent(const T& state) : Event(Events::EventType::COMPONENT_TRIGGER), state(state) {}
    const T getState() const noexcept { return state; }
};

#endif
