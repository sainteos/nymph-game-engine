#ifndef SPRITE_MOVE_EVENT_H
#define SPRITE_MOVE_EVENT_H
#include "events/event.h"
#include <glm/glm.hpp>

class SpriteMoveEvent : public Events::Event {
  private:
    glm::vec2 velocity;
    glm::vec2 next_position;
  public:
    SpriteMoveEvent(const glm::vec2& velocity, const glm::vec2& next_position)
      : Event(Events::EventType::SPRITE_MOVE), velocity(velocity), next_position(next_position) {}
    const glm::vec2 getVelocity() const noexcept { return velocity; }
    const glm::vec2 getNextPosition() const noexcept { return next_position; }
};

#endif
