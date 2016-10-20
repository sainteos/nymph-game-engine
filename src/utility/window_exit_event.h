#ifndef WINDOW_EXIT_EVENT_H
#define WINDOW_EXIT_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  /**
   * @brief      Class for window exit event.
   */
  class WindowExitEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Constructor for WindowExitEvent
       */
      WindowExitEvent() : Event(Events::EventType::WINDOW_EXIT) {}
      /**
       * @brief      Factory Function
       *
       * @return     newly created WindowExitEvent
       */
      static std::shared_ptr<WindowExitEvent> create() { return std::make_shared<WindowExitEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif