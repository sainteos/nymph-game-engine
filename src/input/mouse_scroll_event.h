#ifndef MOUSE_SCROLL_EVENT_H
#define MOUSE_SCROLL_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Input {
  /**
   * @brief      Class for mouse scroll event.
   */
  class MouseScrollEvent : public Events::Event {
    private:
      glm::dvec2 offset;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      MouseScrollEvent constructor
       *
       * @param[in]  offset  The offset
       */
      MouseScrollEvent(const glm::dvec2& offset) : Event(Events::EventType::MOUSE_SCROLL), offset(offset) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  offset  The offset
       *
       * @return     Newly created MouseScrollEvent
       */
      static std::shared_ptr<MouseScrollEvent> create(const glm::dvec2& offset) { return std::make_shared<MouseScrollEvent>(offset); }
      /**
       * @brief      Gets the position.
       *
       * @return     The position.
       */
      const glm::dvec2 getPosition() const noexcept { return offset; }
      //= END SCRIPTABLE
  };
}

#endif
