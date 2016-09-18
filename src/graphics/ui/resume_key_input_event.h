#ifndef RESUME_KEY_INPUT_EVENT_H
#define RESUME_KEY_INPUT_EVENT_H
#include "events/event.h"
#include "events/event_type.h"

namespace Graphics {
  namespace UI {
    class ResumeKeyInputEvent : public Events::Event {
      public:
        ResumeKeyInputEvent() : Event(Events::EventType::RESUME_KEY_INPUT) {}
        static std::shared_ptr<ResumeKeyInputEvent> create() { return std::make_shared<ResumeKeyInputEvent>(); }
    };
  }
}

#endif
