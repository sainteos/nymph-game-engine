#ifndef SPRITE_MOVE_EVENT_H
#define SPRITE_MOVE_EVENT_H
#include "../events/event.h"
#include <glm/glm.hpp>

namespace Game {
  /**
   * @brief      Class for sprite move event.
   */
  class [[scriptable]] SpriteMoveEvent : public Events::Event {
    private:
      glm::vec2 velocity;
      glm::vec2 next_position;
    public:

      /**
       * @brief      SpriteMoveEvent constructor
       *
       * @param[in]  velocity       The velocity
       * @param[in]  next_position  The next position
       */
      [[scriptable]] SpriteMoveEvent(const glm::vec2& velocity, const glm::vec2& next_position) : Event(Events::EventType::SPRITE_MOVE), velocity(velocity), next_position(next_position) {}
      /**
       * @brief      SpriteMoveEvent factory function
       *
       * @param      velocity       The velocity
       * @param[in]  next_position  The next position
       *
       * @return     A newly constructed SpriteMoveEvent
       */
      [[scriptable]] static std::shared_ptr<SpriteMoveEvent> create(glm::vec2& velocity, const glm::vec2& next_position) { return std::make_shared<SpriteMoveEvent>(velocity, next_position); }

      /**
       * @brief      Gets the velocity.
       *
       * @return     The velocity.
       */
      [[scriptable]] glm::vec2 getVelocity() const noexcept { return velocity; }
      /**
       * @brief      Gets the next position.
       *
       * @return     The next position.
       */
      [[scriptable]] glm::vec2 getNextPosition() const noexcept { return next_position; }
  };
}
#endif
