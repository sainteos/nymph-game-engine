#ifndef LIST_CHARACTERS_EVENT_H
#define LIST_CHARACTERS_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for list characters event.
   */
  class [[scriptable]] ListCharactersEvent : public Events::Event {
    public:

      /**
       * @brief      ListCharactersEvent constructor
       */
      [[scriptable]] ListCharactersEvent() : Event(Events::EventType::LIST_CHARACTERS) {}
      /**
       * @brief      Factory function
       *
       * @return     Newly created ListCharacterEvent
       */
      [[scriptable]] static std::shared_ptr<ListCharactersEvent> create() { return std::make_shared<ListCharactersEvent>(); }
  };
}

#endif
