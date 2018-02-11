#ifndef RESUME_KEY_INPUT_EVENT_H
#define RESUME_KEY_INPUT_EVENT_H
#include "../../events/event.h"
#include "../../events/event_type.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for resume key input event.
     */
    class [[scriptable]] ResumeKeyInputEvent : public Events::Event {
      public:

        /**
         * @brief      Resume Key Input Event constructor
         */
        [[scriptable]] ResumeKeyInputEvent() : Event(Events::EventType::RESUME_KEY_INPUT) {}
        /**
         * @brief      Resume Key Input Event factory function
         *
         * @return     A newly created ResumeKeyInputEvent
         */
        [[scriptable]] static std::shared_ptr<ResumeKeyInputEvent> create() { return std::make_shared<ResumeKeyInputEvent>(); }
    };
  }
}

#endif
