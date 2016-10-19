#ifndef SET_ACTIVE_EVENT_H
#define SET_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

class SetActiveEvent : public Events::Event {
  private:
    bool active;
  public:
    //= BEGIN SCRIPTABLE
    SetActiveEvent(const bool active) : Event(Events::EventType::SET_ACTIVE), active(active) {}
    static std::shared_ptr<SetActiveEvent> create(const bool active) { return std::make_shared<SetActiveEvent>(active); }
    const bool getActive() const noexcept { return active; }
    //= END SCRIPTABLE
};

#endif
