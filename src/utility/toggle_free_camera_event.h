#ifndef TOGGLE_FREE_CAMERA_EVENT_H
#define TOGGLE_FREE_CAMERA_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ToggleFreeCameraEvent : public Events::Event {
    private:
      bool on;
    public:
      ToggleFreeCameraEvent(const bool on) : Event(Events::EventType::TOGGLE_FREE_CAMERA), on(on) {}
      static std::shared_ptr<ToggleFreeCameraEvent> create(const bool on) { return std::make_shared<ToggleFreeCameraEvent>(on); }
      const bool turnOn() const noexcept { return on; }
  };
}

#endif