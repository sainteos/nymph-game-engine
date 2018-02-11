#ifndef TOGGLE_FREE_CAMERA_EVENT_H
#define TOGGLE_FREE_CAMERA_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  /**
   * @brief      Class for toggle free camera event.
   */
  class [[scriptable]] ToggleFreeCameraEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      ToggleFreeCameraEvent constructor
       */
      [[scriptable]] ToggleFreeCameraEvent() : Event(Events::EventType::TOGGLE_FREE_CAMERA) {}
      /**
       * @brief      Factory function
       *
       * @return     newly created ToggleFreeCameraEvent
       */
      [[scriptable]] static std::shared_ptr<ToggleFreeCameraEvent> create() { return std::make_shared<ToggleFreeCameraEvent>(); }
  };
}

#endif
