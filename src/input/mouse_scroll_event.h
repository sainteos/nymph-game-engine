#ifndef MOUSE_SCROLL_EVENT_H
#define MOUSE_SCROLL_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class MouseScrollEvent : public Events::Event {
    private:
      glm::dvec2 offset;
    public:
      MouseScrollEvent(const glm::dvec2& offset) : Event(Events::EventType::MOUSE_SCROLL), offset(offset) {}
      static std::shared_ptr<MouseScrollEvent> create(const glm::dvec2& offset) { return std::make_shared<MouseScrollEvent>(offset); }
      const glm::dvec2 getPosition() const noexcept { return offset; }
  };
}

#endif
