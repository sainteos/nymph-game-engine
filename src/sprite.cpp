#include "sprite.h"
#include "events/event_type.h"
#include "input/key_down_event.h"
#include "input/key_up_event.h"
#include "sprite_move_event.h"
#include <easylogging++.h>
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include <glm/ext.hpp>

Sprite::Sprite() : current_state(AnimationState::FACE_DOWN), move_quantization_in_tiles(1.0),
  up_down(false), down_down(false), left_down(false), right_down(false) {

}

void Sprite::onNotify(const Events::Event& event) {
  if(event.getEventCode() == Events::EventType::KEY_DOWN) {
    auto casted_event = static_cast<const Input::KeyDownEvent*>(&event);
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
  }
  else if(event.getEventCode() == Events::EventType::KEY_UP) {
    auto casted_event = static_cast<const Input::KeyUpEvent*>(&event);
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
  }
}

void Sprite::onStart() {
  for(auto i : tiles) {
    if(i.first == current_state)
      i.second->setActive();
    else
      i.second->setInactive();
  }
  Entity::onStart();
}

void Sprite::onUpdate(const float delta) {
  if(current_state == AnimationState::FACE_LEFT || current_state == AnimationState::FACE_RIGHT ||
     current_state == AnimationState::FACE_UP || current_state == AnimationState::FACE_DOWN) {
    if(left_down) {
      current_velocity = glm::vec2(-moving_speed, 0.0);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      triggerTile(AnimationState::MOVE_LEFT);
      notify(SpriteMoveEvent(current_velocity, absolute_next_position));
    }
    else if(right_down) {
      current_velocity = glm::vec2(moving_speed, 0.0);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      triggerTile(AnimationState::MOVE_RIGHT);
      notify(SpriteMoveEvent(current_velocity, absolute_next_position));
    } 
    else if(up_down) {
      current_velocity = glm::vec2(0.0, moving_speed);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      triggerTile(AnimationState::MOVE_UP);
      notify(SpriteMoveEvent(current_velocity, absolute_next_position));
    }
    else if(down_down) {
      current_velocity = glm::vec2(0.0, -moving_speed);
      next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      auto absolute_next_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
      triggerTile(AnimationState::MOVE_DOWN);
      notify(SpriteMoveEvent(current_velocity, absolute_next_position));
    }
  }
  if(current_state == AnimationState::MOVE_LEFT || current_state == AnimationState::MOVE_RIGHT ||
     current_state == AnimationState::MOVE_UP || current_state == AnimationState::MOVE_DOWN) {
    if(glm::distance(glm::vec2(getTransform()->getLocalTranslation()), next_position) < 1.0f / 1000.0f * delta) {
      getTransform()->translate(next_position - glm::vec2(getTransform()->getLocalTranslation()));
      if(current_state == AnimationState::MOVE_LEFT) {
        current_velocity = glm::vec2(0.0, 0.0);
        triggerTile(AnimationState::FACE_LEFT);
      }
      else if(current_state == AnimationState::MOVE_RIGHT) {
        current_velocity = glm::vec2(0.0, 0.0);
        triggerTile(AnimationState::FACE_RIGHT);
      }
      else if(current_state == AnimationState::MOVE_UP) {
        current_velocity = glm::vec2(0.0, 0.0);
        triggerTile(AnimationState::FACE_UP);
      }
      else if(current_state == AnimationState::MOVE_DOWN) {
        current_velocity = glm::vec2(0.0, 0.0);
        triggerTile(AnimationState::FACE_DOWN);
      }
    }
    else {
      getTransform()->translate(current_velocity * 1.0 / 1000.0f * delta);
    }
  }
  Entity::onUpdate(delta);
}

void Sprite::addTile(const AnimationState& state, std::shared_ptr<Graphics::Tile> tile) {
  addComponent(tile);
  tiles[state] = tile;
}

void Sprite::triggerTile(const AnimationState& state) {
  tiles[current_state]->setInactive();
  current_state = state;
  tiles[current_state]->setActive();
}

void Sprite::setMovingSpeed(const float speed) {
  moving_speed = speed;
}

void Sprite::setMoveQuantization(const float number_of_tiles) {
  move_quantization_in_tiles = number_of_tiles;
}

void Sprite::stopMovingLeft() {
  left_down = false;
}

void Sprite::moveLeft() {
  left_down = true;
}

void Sprite::stopMovingRight() {
  right_down = false;
}

void Sprite::moveRight() {
  right_down = true;
}

void Sprite::stopMovingUp() {
  up_down = false;
}

void Sprite::moveUp() {
  up_down = true;
}

void Sprite::stopMovingDown() {
  down_down = false;
}

void Sprite::moveDown() {
  down_down = true;
}
