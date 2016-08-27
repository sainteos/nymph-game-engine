#include "sprite_movement.h"
#include "events/event_type.h"
#include "input/key_down_event.h"
#include "input/key_up_event.h"
#include "sprite_move_event.h"
#include "animation_trigger_event.h"
#include <easylogging++.h>
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include <glm/ext.hpp>

SpriteMovement::SpriteMovement() : move_quantization_in_tiles(1.0),
  up_down(false), down_down(false), left_down(false), right_down(false), current_state(SpriteState::FACE_DOWN) {

}

void SpriteMovement::onStart() {
}

const bool SpriteMovement::onUpdate(const double delta) {
  if(!active)
    return false;

  while(eventsWaiting()) {
    handleQueuedEvent(getEvent());
  }

  if(current_state == SpriteState::FACE_LEFT || current_state == SpriteState::FACE_RIGHT ||
     current_state == SpriteState::FACE_UP || current_state == SpriteState::FACE_DOWN) {
    if(left_down) {
      current_velocity = glm::vec2(-moving_speed, 0.0);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      current_state = SpriteState::MOVE_LEFT;
      notifyNow(AnimationTriggerEvent::create(states[SpriteState::MOVE_LEFT]));
      notify(SpriteMoveEvent::create(current_velocity, absolute_next_position));
    }
    else if(right_down) {
      current_velocity = glm::vec2(moving_speed, 0.0);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      current_state = SpriteState::MOVE_RIGHT;
      notifyNow(AnimationTriggerEvent::create(states[SpriteState::MOVE_RIGHT]));
      notify(SpriteMoveEvent::create(current_velocity, absolute_next_position));
    } 
    else if(up_down) {
      current_velocity = glm::vec2(0.0, moving_speed);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      current_state = SpriteState::MOVE_UP;
      notifyNow(AnimationTriggerEvent::create(states[SpriteState::MOVE_UP]));
      notify(SpriteMoveEvent::create(current_velocity, absolute_next_position));
    }
    else if(down_down) {
      current_velocity = glm::vec2(0.0, -moving_speed);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      current_state = SpriteState::MOVE_DOWN;
      notifyNow(AnimationTriggerEvent::create(states[SpriteState::MOVE_DOWN]));
      notify(SpriteMoveEvent::create(current_velocity, absolute_next_position));
    }
  }
  if(current_state == SpriteState::MOVE_LEFT || current_state == SpriteState::MOVE_RIGHT ||
     current_state == SpriteState::MOVE_UP || current_state == SpriteState::MOVE_DOWN) {
    if(glm::distance(glm::vec2(getTransform()->getLocalTranslation()), next_position) > glm::length(current_velocity * 1.0f / 1000.0f * delta)) {
      getTransform()->translate(current_velocity * 1.0 / 1000.0f * delta);
    }
    else {
      getTransform()->translate(next_position - glm::vec2(getTransform()->getLocalTranslation()));
      if(current_state == SpriteState::MOVE_LEFT) {
        current_velocity = glm::vec2(0.0, 0.0);
        current_state = SpriteState::FACE_LEFT;
        notifyNow(AnimationTriggerEvent::create(states[SpriteState::FACE_LEFT]));
      }
      else if(current_state == SpriteState::MOVE_RIGHT) {
        current_velocity = glm::vec2(0.0, 0.0);
        current_state = SpriteState::FACE_RIGHT;
        notifyNow(AnimationTriggerEvent::create(states[SpriteState::FACE_RIGHT]));
      }
      else if(current_state == SpriteState::MOVE_UP) {
        current_velocity = glm::vec2(0.0, 0.0);
        current_state = SpriteState::FACE_UP;
        notifyNow(AnimationTriggerEvent::create(states[SpriteState::FACE_UP]));
      }
      else if(current_state == SpriteState::MOVE_DOWN) {
        current_velocity = glm::vec2(0.0, 0.0);
        current_state = SpriteState::FACE_DOWN;
        notifyNow(AnimationTriggerEvent::create(states[SpriteState::FACE_DOWN]));
      }
    }
  }
  return true;
}

void SpriteMovement::onNotifyNow(std::shared_ptr<Events::Event> event) {
  handleQueuedEvent(event);
}

void SpriteMovement::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
  switch(event->getEventType()) {
    case Events::EventType::KEY_DOWN: {
      auto casted_event = std::static_pointer_cast<Input::KeyDownEvent>(event);
      switch(casted_event->getKey()) {
        case GLFW_KEY_W:
          moveUp();
          break;

        case GLFW_KEY_S:
          moveDown();
          break;

        case GLFW_KEY_A:
          moveLeft();
          break;

        case GLFW_KEY_D:
          moveRight();
          break;
      }
      break;
    }
    case Events::EventType::KEY_UP: {
      auto casted_event = std::static_pointer_cast<Input::KeyUpEvent>(event);
      switch(casted_event->getKey()) {
        case GLFW_KEY_W:
          if(up_down)
            stopMovingUp();
          break;

        case GLFW_KEY_S:
          if(down_down)
            stopMovingDown();
          break;

        case GLFW_KEY_A:
          if(left_down)
            stopMovingLeft();
          break;

        case GLFW_KEY_D:
          if(right_down)
            stopMovingRight();
          break;
      }
      break;
    }
    default:
      Component::handleQueuedEvent(event);
  }
}

void SpriteMovement::setAnimationStringState(const SpriteState& state, const std::string& str) {
  states[state] = str;
}

void SpriteMovement::setMovingSpeed(const float speed) {
  moving_speed = speed;
}

void SpriteMovement::setMoveQuantization(const float number_of_tiles) {
  move_quantization_in_tiles = number_of_tiles;
}

void SpriteMovement::stopMovingLeft() {
  left_down = false;
}

void SpriteMovement::moveLeft() {
  left_down = true;
}

void SpriteMovement::stopMovingRight() {
  right_down = false;
}

void SpriteMovement::moveRight() {
  right_down = true;
}

void SpriteMovement::stopMovingUp() {
  up_down = false;
}

void SpriteMovement::moveUp() {
  up_down = true;
}

void SpriteMovement::stopMovingDown() {
  down_down = false;
}

void SpriteMovement::moveDown() {
  down_down = true;
}
