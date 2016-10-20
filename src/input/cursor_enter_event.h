#ifndef CURSOR_ENTER_EVENT_H
#define CURSOR_ENTER_EVENT_H
#include <glm/glm.hpp>
#include <memory>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Input {
  /**
   * @brief      Class for cursor enter event.
   */
  class CursorEnterEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      CursorEnterEvent constructor
       */
      CursorEnterEvent() : Event(Events::EventType::CURSOR_ENTER) {}
      /**
       * @brief      Factory function
       *
       * @return     Newly created CursorEnterEvent
       */
      static std::shared_ptr<CursorEnterEvent> create() { return std::make_shared<CursorEnterEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif
