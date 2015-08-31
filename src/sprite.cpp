#include "sprite.h"
#include "events/event_type.h"
#include "input/key_down_event.h"
#include "input/key_up_event.h"
#include <easylogging++.h>
#include <glfw3.h>
#include <glm/ext.hpp>

Sprite::Sprite() : current_tile("stop down"), move_quantization_in_tiles(1.0),
  moving_left(false), moving_right(false), moving_up(false), moving_down(false),
  up_down(false), down_down(false), left_down(false), right_down(false) {

}

void Sprite::onNotify(const Events::Event& event) {
  if(event.getEventCode() == Events::EventType::KEY_DOWN) {
    auto casted_event = static_cast<const Input::KeyDownEvent*>(&event);
    switch(casted_event->getKey()) {
      case GLFW_KEY_W:
        if(!moving_up)
          moveUp();
        break;

      case GLFW_KEY_S:
        if(!moving_down)
          moveDown();
        break;

      case GLFW_KEY_A:
        if(!moving_left)
          moveLeft();
        break;

      case GLFW_KEY_D:
        if(!moving_right)
          moveRight();
        break;
    }
  }
  else if(event.getEventCode() == Events::EventType::KEY_UP) {
    auto casted_event = static_cast<const Input::KeyUpEvent*>(&event);
    switch(casted_event->getKey()) {
      case GLFW_KEY_W:
        if(moving_up)
          stopMovingUp();
        break;

      case GLFW_KEY_S:
        if(moving_down)
          stopMovingDown();
        break;

      case GLFW_KEY_A:
        if(moving_left)
          stopMovingLeft();
        break;

      case GLFW_KEY_D:
        if(moving_right)
          stopMovingRight();
        break;
    }
  }
}

void Sprite::onStart() {
  tiles[current_tile]->setActive();
  Entity::onStart();
}

//Fix to actually use delta
void Sprite::onUpdate(const float delta) {
  if(left_down && !moving_left) {
    moving_left = true;
    current_velocity = glm::vec2(-moving_speed, 0.0);
    next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
  }
  else if(right_down && !moving_right) {
    moving_right = true;
    current_velocity = glm::vec2(moving_speed, 0.0);
    next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
  } 
  else if(up_down && !moving_up) {
    moving_up = true;
    current_velocity = glm::vec2(0.0, moving_speed);
    next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
  }
  else if(down_down && !moving_down) {
    moving_down = true;
    current_velocity = glm::vec2(0.0, -moving_speed);
    next_position = glm::vec2(getTransform()->getLocalTranslation()) + glm::normalize(current_velocity) * move_quantization_in_tiles;
  }

  if(moving_left || moving_right || moving_up || moving_down) {
    if(glm::distance(glm::vec2(getTransform()->getLocalTranslation()), next_position) < 1.0f / 1000.0f * 16.6f) {
      getTransform()->translate(next_position - glm::vec2(getTransform()->getLocalTranslation()));
      if(moving_left) {
        moving_left = false;
      }
      if(moving_right) {
        moving_right = false;
      }
      if(moving_up) {
        moving_up = false;
      }
      if(moving_down) {
        moving_down = false;
      }
    }
    else {
      getTransform()->translate(current_velocity * 1.0 / 1000.0f * 16.6f);
    }
  }
  Entity::onUpdate(delta);
}

void Sprite::addTile(const std::string& name, std::shared_ptr<Graphics::Tile> tile) {
  addComponent(tile);
  tiles[name] = tile;
}

void Sprite::triggerTile(const std::string& name) {
  current_tile = name;
}

void Sprite::setMovingSpeed(const float speed) {
  moving_speed = speed;
}

void Sprite::setMoveQuantization(const float number_of_tiles) {
  move_quantization_in_tiles = number_of_tiles;
}

void Sprite::stopMovingLeft() {
  triggerTile("stop left");
  left_down = false;
}

void Sprite::moveLeft() {
  triggerTile("move left");
  left_down = true;
}

void Sprite::stopMovingRight() {
  triggerTile("stop right");
  right_down = false;
}

void Sprite::moveRight() {
  triggerTile("move right");
  right_down = true;
}

void Sprite::stopMovingUp() {
  triggerTile("stop up");
  up_down = false;
}

void Sprite::moveUp() {
  triggerTile("move up");
  up_down = true;
}

void Sprite::stopMovingDown() {
  triggerTile("stop down");
  down_down = false;
}

void Sprite::moveDown() {
  triggerTile("move down");
  down_down = true;
}
