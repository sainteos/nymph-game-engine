#include "sprite_movement.h"
#include "events/event_type.h"
#include "input/key_down_event.h"
#include "input/key_up_event.h"
#include "sprite_move_event.h"
#include "animation_trigger_event.hpp"
#include <easylogging++.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

namespace Game {
  SpriteMovement::SpriteMovement() : data(SpriteMovementMotor::defaultSpriteData()) {
    state_machine = std::unique_ptr<SpriteMovementMotor::SpriteFSM>(new SpriteMovementMotor::SpriteFSM(data, SpriteMovementMotor::FACE_DOWN, this));
    data->transform = getTransform();
    data->current_level = 0;
  }

  void SpriteMovement::addCollisionData(std::shared_ptr<Physics::CollisionData> collision_data) {
    data->collision_data = collision_data;
  }

  void SpriteMovement::onStart() {
    data->tile_location = glm::ivec2(getTransform()->getAbsoluteTranslation().x, -getTransform()->getAbsoluteTranslation().y);

    data->tile_location += glm::ivec2(data->collision_data->getWidth() / 2, data->collision_data->getHeight() / 2 - 1);
    state_machine->start();
  }

  bool SpriteMovement::onUpdate(const double delta) {
    if(!active)
      return false;
    while(eventsWaiting()) {
      handleQueuedEvent(getEvent());
    }

    state_machine->update(delta);

    return true;
  }

  void SpriteMovement::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void SpriteMovement::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    switch(event->getEventType()) {
      case Events::EventType::KEY_DOWN:
      case Events::EventType::KEY_REPEAT: {
        auto casted_event = std::static_pointer_cast<Input::KeyDownEvent>(event);
        switch(casted_event->getKey()) {
          case GLFW_KEY_W:
            state_machine->transition(SpriteMovementMotor::UP);
            break;

          case GLFW_KEY_S:
            state_machine->transition(SpriteMovementMotor::DOWN);
            break;

          case GLFW_KEY_A:
            state_machine->transition(SpriteMovementMotor::LEFT);
            break;

          case GLFW_KEY_D:
            state_machine->transition(SpriteMovementMotor::RIGHT);
            break;
        }
        break;
      }
      default:
        Component::handleQueuedEvent(event);
    }
  }

  unsigned long long SpriteMovement::getValueForSorting() const noexcept {
    return getId();
  }

  void SpriteMovement::setMovingSpeed(const float speed) {
    data->moving_speed = speed;
  }

  void SpriteMovement::setMoveQuantization(const float number_of_tiles) {
    data->move_quantization_in_tiles = number_of_tiles;
  }

  void SpriteMovement::log(el::base::type::ostream_t& os) const {
    os << to_string();
  }

  std::string SpriteMovement::className() const noexcept {
    return "SpriteMovement";
  }

  std::string SpriteMovement::to_string() const noexcept {
    std::stringstream str;
    str << Component::to_string();
    str << " Moving Speed: "<<data->moving_speed<<" Move Quantization (tiles): "<<data->move_quantization_in_tiles<<" Current Velocity: "<<glm::to_string(data->current_velocity)<<" Next Position: "<<glm::to_string(data->next_position);
    return str.str();
  }
}
