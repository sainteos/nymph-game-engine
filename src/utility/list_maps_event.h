#ifndef LIST_MAPS_EVENT_H
#define LIST_MAPS_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for list maps event.
   */
  class [[scriptable]] ListMapsEvent : public Events::Event {
    public:

      /**
       * @brief      ListMapsEvent constructor
       */
      [[scriptable]] ListMapsEvent() : Event(Events::EventType::LIST_MAPS) {}
      /**
       * @brief      Factory function
       *
       * @return     A newly constructed ListMapsEvent
       */
      [[scriptable]] static std::shared_ptr<ListMapsEvent> create() { return std::make_shared<ListMapsEvent>(); }
  };
}

#endif
