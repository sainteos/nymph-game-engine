#ifndef MOUSE_BUTTON_DOWN_EVENT_H
#define MOUSE_BUTTON_DOWN_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class MouseButtonDownEvent : public Events::Event {
    private:
      int button;
    public:
      MouseButtonDownEvent(const int button) : Event(Events::EventType::MOUSE_BUTTON_DOWN), button(button) {}
      static std::shared_ptr<MouseButtonDownEvent> create(const int button) { return std::make_shared<MouseButtonDownEvent>(button); }
      const int getButton() const noexcept { return button; }
  };
}

#endif
