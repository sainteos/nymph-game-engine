#ifndef SET_ACTIVE_EVENT_H
#define SET_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

class SetActiveEvent : public Events::Event {
  private:
    unsigned int component_id;
    bool active;
  public:
    SetActiveEvent(const unsigned int id, const bool active) : Event(Events::EventType::SET_ACTIVE), component_id(id), active(active) {}
    static std::shared_ptr<SetActiveEvent> create(const unsigned int id, const bool active) { return std::make_shared<SetActiveEvent>(id, active); }
    const unsigned getComponentId() const noexcept { return component_id; }
    const bool getActive() const noexcept { return active; }
};

#endif
