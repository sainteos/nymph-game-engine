#ifndef MOUSE_CURSOR_EVENT_H
#define MOUSE_CURSOR_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  class MouseCursorEvent : public Events::Event {
    private:
      glm::dvec2 position;
    public:
      //= BEGIN SCRIPTABLE
      MouseCursorEvent(const glm::dvec2& pos) : Event(Events::EventType::MOUSE_CURSOR), position(pos) {}
      static std::shared_ptr<MouseCursorEvent> create(const glm::dvec2& pos) { return std::make_shared<MouseCursorEvent>(pos); }
      const glm::dvec2 getPosition() const noexcept { return position; }
      //= END SCRIPTABLE
  };
}
#endif
