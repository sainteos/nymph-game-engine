#ifndef SET_ENTITY_ACTIVE_EVENT_H
#define SET_ENTITY_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

class SetEntityActiveEvent : public Events::Event {
  private:
    bool active;
  public:
    SetEntityActiveEvent(const bool active) : Event(Events::EventType::SET_ENTITY_ACTIVE), active(active) {}
    static std::shared_ptr<SetEntityActiveEvent> create(const bool active) { return std::make_shared<SetEntityActiveEvent>(active); }
    const bool getActive() const noexcept { return active; }
};

#endif
