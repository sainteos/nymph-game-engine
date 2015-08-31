#ifndef CURSOR_LEAVE_EVENT_H
#define CURSOR_LEAVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class CursorLeaveEvent : public Events::Event {
    public:
      CursorLeaveEvent() : Event(Events::EventType::CURSOR_LEAVE) {}
  };
}

#endif