#ifndef SUSPEND_KEY_INPUT_EVENT_H
#define SUSPEND_KEY_INPUT_EVENT_H
#include "../../events/event.h"
#include "../../events/event_type.h"
namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for suspend key input event.
     */
    class [[scriptable]] SuspendKeyInputEvent : public Events::Event {
      public:

        /**
         * @brief      Constructor for SuspendKeyInputEvent
         */
        [[scriptable]] SuspendKeyInputEvent() : Event(Events::EventType::SUSPEND_KEY_INPUT) {}
        /**
         * @brief      Factory function for SuspendKeyInputEvent
         *
         * @return     A newly constructed SuspendKeyInputEvent
         */
        [[scriptable]] static std::shared_ptr<SuspendKeyInputEvent> create() { return std::make_shared<SuspendKeyInputEvent>(); }
    };
  }
}

#endif
