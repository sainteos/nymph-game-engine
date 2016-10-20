#ifndef LIST_MAPS_EVENT_H
#define LIST_MAPS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Utility {
  /**
   * @brief      Class for list maps event.
   */
  class ListMapsEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      ListMapsEvent constructor
       */
      ListMapsEvent() : Event(Events::EventType::LIST_MAPS) {}
      /**
       * @brief      Factory function
       *
       * @return     A newly constructed ListMapsEvent
       */
      static std::shared_ptr<ListMapsEvent> create() { return std::make_shared<ListMapsEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif