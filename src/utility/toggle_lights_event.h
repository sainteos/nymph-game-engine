#ifndef TOGGLE_LIGHTS_EVENT_H
#define TOGGLE_LIGHTS_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  class ToggleLightsEvent : public Events::Event {
    private:
      bool on;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      ToggleLightsEvent constructor
       *
       * @param[in]  on    Turn the lights on?
       */
      ToggleLightsEvent(const bool on) : Event(Events::EventType::TOGGLE_LIGHTS), on(on) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  on    Turn the lights on?
       *
       * @return     newly created TogglLightsEvent
       */
      static std::shared_ptr<ToggleLightsEvent> create(const bool on) { return std::make_shared<ToggleLightsEvent>(on); }
      /**
       * @brief      Turn the lights on?
       *
       * @return     True if yes
       */
      bool turnOn() const noexcept { return on; }
      //= END SCRIPTABLE
  };
}

#endif
