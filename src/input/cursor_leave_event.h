#ifndef CURSOR_LEAVE_EVENT_H
#define CURSOR_LEAVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  class CursorLeaveEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      CursorLeaveEvent() : Event(Events::EventType::CURSOR_LEAVE) {}
      static std::shared_ptr<CursorLeaveEvent> create() { return std::make_shared<CursorLeaveEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif