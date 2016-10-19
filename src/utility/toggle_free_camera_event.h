#ifndef TOGGLE_FREE_CAMERA_EVENT_H
#define TOGGLE_FREE_CAMERA_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  class ToggleFreeCameraEvent : public Events::Event {
    public:
      //= BEGIN SCRIPTABLE
      ToggleFreeCameraEvent() : Event(Events::EventType::TOGGLE_FREE_CAMERA) {}
      static std::shared_ptr<ToggleFreeCameraEvent> create() { return std::make_shared<ToggleFreeCameraEvent>(); }
      //= END SCRIPTABLE
  };
}

#endif