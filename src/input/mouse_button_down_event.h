#ifndef MOUSE_BUTTON_DOWN_EVENT_H
#define MOUSE_BUTTON_DOWN_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Input {
  /**
   * @brief      Class for mouse button down event.
   */
  class MouseButtonDownEvent : public Events::Event {
    private:
      int button;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      MouseButtonDownEvent constructor
       *
       * @param[in]  button  The button
       */
      MouseButtonDownEvent(const int button) : Event(Events::EventType::MOUSE_BUTTON_DOWN), button(button) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  button  The button
       *
       * @return     A newly created MouseButtonDownEvent
       */
      static std::shared_ptr<MouseButtonDownEvent> create(const int button) { return std::make_shared<MouseButtonDownEvent>(button); }
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
