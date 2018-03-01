#ifndef CURSOR_LEAVE_EVENT_H
#define CURSOR_LEAVE_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Input {
  /**
   * @brief      Class for cursor leave event.
   */
  class [[scriptable]] CursorLeaveEvent : public Events::Event {
    public:

      /**
       * @brief      CusrorLeaveEvent constructor
       */
      [[scriptable]] CursorLeaveEvent() : Event(Events::EventType::CURSOR_LEAVE) {}
      /**
       * @brief      Factory function
       *
       * @return     A newly constructed CursorLeaveEvent
       */
      [[scriptable]] static std::shared_ptr<CursorLeaveEvent> create() { return std::make_shared<CursorLeaveEvent>(); }
  };
}

#endif
