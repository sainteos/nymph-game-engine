#ifndef MOUSE_CURSOR_EVENT_H
#define MOUSE_CURSOR_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  /**
   * @brief      Class for mouse cursor event.
   */
  class MouseCursorEvent : public Events::Event {
    private:
      glm::dvec2 position;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      MouseCursorEvent constructor
       *
       * @param[in]  pos   The position
       */
      MouseCursorEvent(const glm::dvec2& pos) : Event(Events::EventType::MOUSE_CURSOR), position(pos) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  pos   The position
       *
       * @return     Newly created MouseCursorEvent
       */
      static std::shared_ptr<MouseCursorEvent> create(const glm::dvec2& pos) { return std::make_shared<MouseCursorEvent>(pos); }
      /**
       * @brief      Gets the position.
       *
       * @return     The position.
       */
      glm::dvec2 getPosition() const noexcept { return position; }
      //= END SCRIPTABLE
  };
}
#endif
