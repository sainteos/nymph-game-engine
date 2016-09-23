#ifndef TOGGLE_LAYER_EVENT_H
#define TOGGLE_LAYER_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class ToggleLayerEvent : public Events::Event {
    private:
      unsigned int layer_number;
      bool on;
    public:
      ToggleLayerEvent(const unsigned int layer_number, const bool on) : Event(Events::EventType::TOGGLE_LAYER), layer_number(layer_number), on(on) {}
      static std::shared_ptr<ToggleLayerEvent> create(const unsigned int layer_number, const bool on) { return std::make_shared<ToggleLayerEvent>(layer_number, on); }
      const bool turnOn() const noexcept { return on; }
      const unsigned int getLayerNumber() const noexcept { return layer_number; }
  };
}

#endif