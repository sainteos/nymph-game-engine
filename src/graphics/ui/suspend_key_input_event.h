#ifndef SUSPEND_KEY_INPUT_EVENT_H
#define SUSPEND_KEY_INPUT_EVENT_H
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Graphics {
  namespace UI {
    class SuspendKeyInputEvent : public Events::Event {
      public:
      //= BEGIN SCRIPTABLE
        SuspendKeyInputEvent() : Event(Events::EventType::SUSPEND_KEY_INPUT) {}
        static std::shared_ptr<SuspendKeyInputEvent> create() { return std::make_shared<SuspendKeyInputEvent>(); }
      //= END SCRIPTABLE
    };
  }
}

#endif
