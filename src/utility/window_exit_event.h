#ifndef WINDOW_EXIT_EVENT_H
#define WINDOW_EXIT_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for window exit event.
   */
  class [[scriptable]] WindowExitEvent : public Events::Event {
    public:

      /**
       * @brief      Constructor for WindowExitEvent
       */
      [[scriptable]] WindowExitEvent() : Event(Events::EventType::WINDOW_EXIT) {}
      /**
       * @brief      Factory Function
       *
       * @return     newly created WindowExitEvent
       */
      [[scriptable]] static std::shared_ptr<WindowExitEvent> create() { return std::make_shared<WindowExitEvent>(); }
  };
}

#endif
