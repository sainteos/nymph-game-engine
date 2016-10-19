#ifndef ANIMATION_TRIGGER_EVENT_H
#define ANIMATION_TRIGGER_EVENT_H
#include <glm/glm.hpp>
#include <easylogging++.h>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Game {
  class AnimationTriggerEvent : public Events::Event {
    private:
      std::string state;
    public:
      //= BEGIN SCRIPTABLE
      AnimationTriggerEvent(const std::string& state) : Event(Events::EventType::ANIMATION_TRIGGER), state(state) {}
      static std::shared_ptr<AnimationTriggerEvent> create(const std::string& state) { return std::make_shared<AnimationTriggerEvent>(state); }
      const std::string getState() const noexcept { return state; }
      //= END SCRIPTABLE
  }; 
}

#endif
