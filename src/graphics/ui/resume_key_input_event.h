#ifndef RESUME_KEY_INPUT_EVENT_H
#define RESUME_KEY_INPUT_EVENT_H
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for resume key input event.
     */
    class ResumeKeyInputEvent : public Events::Event {
      public:
      //= BEGIN SCRIPTABLE
        
        /**
         * @brief      Resume Key Input Event constructor
         */
        ResumeKeyInputEvent() : Event(Events::EventType::RESUME_KEY_INPUT) {}
        /**
         * @brief      Resume Key Input Event factory function
         *
         * @return     A newly created ResumeKeyInputEvent
         */
        static std::shared_ptr<ResumeKeyInputEvent> create() { return std::make_shared<ResumeKeyInputEvent>(); }
      //= END SCRIPTABLE
    };
  }
}

#endif
