#ifndef LIST_LAYERS_EVENT_H
#define LIST_LAYERS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Utility {
  /**
   * @brief      Class for list layers event.
   */
  class ListLayersEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      ListLayersEvent constructor
       */
      ListLayersEvent() : Event(Events::EventType::LIST_LAYERS) {}
      /**
       * @brief      Factory function
       *
       * @return     Newly constructed ListLayersEvent
       */
      static std::shared_ptr<ListLayersEvent> create() { return std::make_shared<ListLayersEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif