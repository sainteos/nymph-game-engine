#ifndef ANIMATION_TRIGGER_EVENT_H
#define ANIMATION_TRIGGER_EVENT_H
#include <glm/glm.hpp>
#include <easylogging++.h>
#include "events/event.h"
#include "events/event_type.h"

namespace Game {
  /**
   * @brief      Class for animation trigger event.
   */
  template<typename StateType>
  class AnimationTriggerEvent : public Events::Event {
    private:
      StateType state;
    public:
      /**
       * @brief      AnimationTriggerEvent constructor
       *
       * @param[in]  state  The state that the animation will change to
       */
      AnimationTriggerEvent(const StateType& state) : Event(Events::EventType::ANIMATION_TRIGGER), state(state) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  state  The state
       *
       * @return     Shared pointer to a new AnimationTriggerEvent
       */
      static std::shared_ptr<AnimationTriggerEvent<StateType>> create(const StateType& state) { return std::make_shared<AnimationTriggerEvent<StateType>>(state); }
      /**
       * @brief      Gets the state.
       *
       * @return     The state.
       */
      const StateType getState() const noexcept { return state; }
  }; 
}

#endif
