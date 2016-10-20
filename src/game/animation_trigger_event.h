#ifndef ANIMATION_TRIGGER_EVENT_H
#define ANIMATION_TRIGGER_EVENT_H
#include <glm/glm.hpp>
#include <easylogging++.h>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Game {
  /**
   * @brief      Class for animation trigger event.
   */
  class AnimationTriggerEvent : public Events::Event {
    private:
      std::string state;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      AnimationTriggerEvent constructor
       *
       * @param[in]  state  The state that the animation will change to
       */
      AnimationTriggerEvent(const std::string& state) : Event(Events::EventType::ANIMATION_TRIGGER), state(state) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  state  The state
       *
       * @return     Shared pointer to a new AnimationTriggerEvent
       */
      static std::shared_ptr<AnimationTriggerEvent> create(const std::string& state) { return std::make_shared<AnimationTriggerEvent>(state); }
      /**
       * @brief      Gets the state.
       *
       * @return     The state.
       */
      const std::string getState() const noexcept { return state; }
      //= END SCRIPTABLE
  }; 
}

#endif
