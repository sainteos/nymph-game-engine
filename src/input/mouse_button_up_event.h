#ifndef MOUSE_BUTTON_UP_EVENT_H
#define MOUSE_BUTTON_UP_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class MouseButtonUpEvent : public Events::Event {
    private:
      int button;
    public:
      MouseButtonUpEvent(const int button) : Event(Events::EventType::MOUSE_BUTTON_UP), button(button) {}
      static std::shared_ptr<MouseButtonUpEvent> create(const int button) { return std::make_shared<MouseButtonUpEvent>(button); }
      const int getButton() const noexcept { return button; }
  };
}

#endif
