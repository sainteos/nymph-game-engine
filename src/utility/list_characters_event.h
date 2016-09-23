#ifndef LIST_CHARACTERS_EVENT_H
#define LIST_CHARACTERS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ListCharactersEvent : public Events::Event {
    public:
      ListCharactersEvent() : Event(Events::EventType::LIST_CHARACTERS) {}
      static std::shared_ptr<ListCharactersEvent> create() { return std::make_shared<ListCharactersEvent>(); }
  };
}

#endif