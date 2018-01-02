#ifndef MOUSE_BUTTON_UP_EVENT_H
#define MOUSE_BUTTON_UP_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  /**
   * @brief      Class for mouse button up event.
   */
  class MouseButtonUpEvent : public Events::Event {
    private:
      int button;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      MouseButtonUpEvent constructor
       *
       * @param[in]  button  The button
       */
      MouseButtonUpEvent(const int button) : Event(Events::EventType::MOUSE_BUTTON_UP), button(button) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  button  The button
       *
       * @return     A newly constructed MouseButtonUpEvent
       */
      static std::shared_ptr<MouseButtonUpEvent> create(const int button) { return std::make_shared<MouseButtonUpEvent>(button); }
      /**
       * @brief      Gets the button.
       *
       * @return     The button.
       */
      int getButton() const noexcept { return button; }
      //= END SCRIPTABLE
  };
}

#endif
