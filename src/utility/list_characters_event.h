#ifndef LIST_CHARACTERS_EVENT_H
#define LIST_CHARACTERS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Utility {
  /**
   * @brief      Class for list characters event.
   */
  class ListCharactersEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      ListCharactersEvent constructor
       */
      ListCharactersEvent() : Event(Events::EventType::LIST_CHARACTERS) {}
      /**
       * @brief      Factory function
       *
       * @return     Newly created ListCharacterEvent
       */
      static std::shared_ptr<ListCharactersEvent> create() { return std::make_shared<ListCharactersEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif