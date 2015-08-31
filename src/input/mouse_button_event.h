#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class MouseButtonEvent : public Events::Event {
    private:
      int button;
    public:
      MouseButtonEvent(const int button) : Event(Events::EventType::MOUSE_BUTTON), button(button) {}
      const int getButton() const noexcept { return button; }
  };
}

#endif
