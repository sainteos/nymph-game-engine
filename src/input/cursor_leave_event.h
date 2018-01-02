#ifndef CURSOR_LEAVE_EVENT_H
#define CURSOR_LEAVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  /**
   * @brief      Class for cursor leave event.
   */
  class CursorLeaveEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      CusrorLeaveEvent constructor
       */
      CursorLeaveEvent() : Event(Events::EventType::CURSOR_LEAVE) {}
      /**
       * @brief      Factory function
       *
       * @return     A newly constructed CursorLeaveEvent
       */
      static std::shared_ptr<CursorLeaveEvent> create() { return std::make_shared<CursorLeaveEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif
