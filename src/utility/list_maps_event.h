#ifndef LIST_MAPS_EVENT_H
#define LIST_MAPS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ListMapsEvent : public Events::Event {
    public:
      ListMapsEvent() : Event(Events::EventType::LIST_MAPS) {}
      static std::shared_ptr<ListMapsEvent> create() { return std::make_shared<ListMapsEvent>(); }
  };
}

#endif