#ifndef CURSOR_ENTER_EVENT_H
#define CURSOR_ENTER_EVENT_H
#include <glm/glm.hpp>
#include <memory>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class CursorEnterEvent : public Events::Event {
    public:
      CursorEnterEvent() : Event(Events::EventType::CURSOR_ENTER) {}
      static std::shared_ptr<CursorEnterEvent> create() { return std::make_shared<CursorEnterEvent>(); }
  };
}

#endif
