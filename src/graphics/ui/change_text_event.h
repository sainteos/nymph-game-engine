#ifndef CHANGE_TEXT_EVENT_H
#define CHANGE_TEXT_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Graphics {
  namespace UI {
    class ChangeTextEvent : public Events::Event {
      private:
        std::string text;
      public:
        ChangeTextEvent(const std::string& text) : Event(Events::EventType::CHANGE_TEXT), text(text) {}
        static std::shared_ptr<ChangeTextEvent> create(const std::string& text) { return std::make_shared<ChangeTextEvent>(text); }
        const std::string getText() const noexcept { return text; }
    }; 
  }
}

#endif
