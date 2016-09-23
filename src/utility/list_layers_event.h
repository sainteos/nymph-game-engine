#ifndef LIST_LAYERS_EVENT_H
#define LIST_LAYERS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ListLayersEvent : public Events::Event {
    public:
      ListLayersEvent() : Event(Events::EventType::LIST_LAYERS) {}
      static std::shared_ptr<ListLayersEvent> create() { return std::make_shared<ListLayersEvent>(); }
  };
}

#endif