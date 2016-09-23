#ifndef TOGGLE_LIGHTS_EVENT_H
#define TOGGLE_LIGHTS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ToggleLightsEvent : public Events::Event {
    private:
      bool on;
    public:
      ToggleLightsEvent(const bool on) : Event(Events::EventType::TOGGLE_LIGHTS), on(on) {}
      static std::shared_ptr<ToggleLightsEvent> create(const bool on) { return std::make_shared<ToggleLightsEvent>(on); }
      const bool turnOn() const noexcept { return on; }
  };
}

#endif