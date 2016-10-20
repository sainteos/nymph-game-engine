#ifndef SUSPEND_KEY_INPUT_EVENT_H
#define SUSPEND_KEY_INPUT_EVENT_H
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for suspend key input event.
     */
    class SuspendKeyInputEvent : public Events::Event {
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      Constructor for SuspendKeyInputEvent
         */
        SuspendKeyInputEvent() : Event(Events::EventType::SUSPEND_KEY_INPUT) {}
        /**
         * @brief      Factory function for SuspendKeyInputEvent
         *
         * @return     A newly constructed SuspendKeyInputEvent
         */
        static std::shared_ptr<SuspendKeyInputEvent> create() { return std::make_shared<SuspendKeyInputEvent>(); }
      //= END SCRIPTABLE
    };
  }
}

#endif
