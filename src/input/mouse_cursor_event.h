#ifndef MOUSE_CURSOR_EVENT_H
#define MOUSE_CURSOR_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class MouseCursorEvent : public Events::Event {
    private:
      glm::dvec2 position;
    public:
      MouseCursorEvent(const glm::dvec2& pos) : Event(Events::EventType::MOUSE_CURSOR), position(pos) {}
      const glm::dvec2 getPosition() const noexcept { return position; }
  };
}
#endif