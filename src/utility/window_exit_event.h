#ifndef WINDOW_EXIT_EVENT_H
#define WINDOW_EXIT_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  class WindowExitEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      WindowExitEvent() : Event(Events::EventType::WINDOW_EXIT) {}
      static std::shared_ptr<WindowExitEvent> create() { return std::make_shared<WindowExitEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif