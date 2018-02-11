#ifndef CURSOR_ENTER_EVENT_H
#define CURSOR_ENTER_EVENT_H
#include <glm/glm.hpp>
#include <memory>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Input {
  /**
   * @brief      Class for cursor enter event.
   */
  class [[scriptable]] CursorEnterEvent : public Events::Event {
    public:

      /**
       * @brief      CursorEnterEvent constructor
       */
      [[scriptable]] CursorEnterEvent() : Event(Events::EventType::CURSOR_ENTER) {}
      /**
       * @brief      Factory function
       *
       * @return     Newly created CursorEnterEvent
       */
      [[scriptable]] static std::shared_ptr<CursorEnterEvent> create() { return std::make_shared<CursorEnterEvent>(); }
  };
}

#endif
