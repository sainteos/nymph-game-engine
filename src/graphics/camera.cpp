#include <easylogging++.h>
#include "camera.h"
#include "glm/ext.hpp"
#include "graphics/shader_manager.h"
#include "sprite_move_event.h"

namespace Graphics {
  
  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager) : shader_manager(shader_manager), projection_matrix(1.0), screen_padding_in_tiles(2) {

  }

  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near, const float far) 
    : viewport_width(viewport_width), viewport_height(viewport_height), near(near), far(far), projection_matrix(1.0), shader_manager(shader_manager), screen_padding_in_tiles(2) {
  }

  void Camera::onStart() { 
    projection_matrix = glm::ortho(viewport_width / -2.0f, viewport_width / 2.0f, viewport_height / -2.0f, viewport_height / 2.0f, near, far);
    shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);

    shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(getTransform()).getAbsoluteTransformationMatrix());
    last_projection_matrix = projection_matrix;
    last_transform = *getTransform();
    target_position = glm::vec2(getTransform()->getLocalTranslation());
    velocity = glm::vec2(0.0, 0.0);
  }

  const bool Camera::onUpdate(const double delta) {
    if(!active)
      return false;
    
    while(eventsWaiting()) {
      handleQueuedEvent(getEvent());
    }
    if(velocity != glm::vec2(0.0, 0.0)) {
      if(glm::distance(target_position, glm::vec2(getTransform()->getLocalTranslation())) < 1.0 / 1000.0 * delta) {
        getTransform()->translate(target_position - glm::vec2(getTransform()->getLocalTranslation()));
        velocity = glm::vec2(0.0, 0.0);
      }
      else {
        getTransform()->translate(velocity * 1.0 / 1000.0 * delta);
      }
    }
    if(projection_matrix != last_projection_matrix) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);
      last_projection_matrix = projection_matrix;
    }

    if(last_transform != *getTransform()) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(getTransform()).getAbsoluteTransformationMatrix());
      last_transform = *getTransform();
    }
    return true;
  }

  void Camera::onDestroy() {
  }

  void Camera::handleQueuedEvent(std::shared_ptr<Events::Event> event) { 
    switch(event->getEventType()) {
      case Events::EventType::SPRITE_MOVE: {
        auto casted_event = std::static_pointer_cast<SpriteMoveEvent>(event);

        if(casted_event->getVelocity().x > 0.0) {
          if(casted_event->getNextPosition().x > getTransform()->getAbsoluteTranslation().x + viewport_width / 2.0 - screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(1.0, 0.0);
          }
        }
        else if(casted_event->getVelocity().x < 0.0) {
          if(casted_event->getNextPosition().x < getTransform()->getAbsoluteTranslation().x - viewport_width / 2.0 + screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(-1.0, 0.0);
          }
        }
        else if(casted_event->getVelocity().y > 0.0) {
          if(casted_event->getNextPosition().y > getTransform()->getAbsoluteTranslation().y + viewport_height / 2.0 - screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(0.0, 1.0);
          }
        }
        else if(casted_event->getVelocity().y < 0.0) {
          if(casted_event->getNextPosition().y < getTransform()->getAbsoluteTranslation().y - viewport_height / 2.0 + screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(0.0, -1.0);
          }
        }
        break;
      }
      default:
        Component::handleQueuedEvent(event);
    }
  }

  void Camera::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void Camera::setScreenPaddingInTiles(const int padding) noexcept {
    screen_padding_in_tiles = padding;
  }

  const int Camera::getScreenPaddingInTiles() const noexcept {
    return screen_padding_in_tiles;
  }

  void Camera::setWidth(const float width) noexcept {
    viewport_width = width;
  }

  const float Camera::getWidth() const noexcept {
    return viewport_width;
  }

  void Camera::setHeight(const float height) noexcept {
    viewport_height = height;
  }

  const float Camera::getHeight() const noexcept {
    return viewport_height;
  }

  void Camera::setNear(const float near) noexcept {
    this->near = near;
  }

  const float Camera::getNear() const noexcept {
    return near;
  }

  void Camera::setFar(const float far) noexcept {
    this->far = far;
  }

  const float Camera::getFar() const noexcept {
    return far;
  }

  const bool Camera::isComponentWithin(const Component& component) const {
    auto translation = component.getTransform()->getAbsoluteTranslation();
    auto camera_translation = getTransform()->getAbsoluteTranslation();
    
    auto camera_bound_left = -(viewport_width + 2.0) / 2.0 + camera_translation.x;
    auto camera_bound_right = (viewport_width + 2.0) / 2.0 + camera_translation.x;
    auto camera_bound_bottom = -(viewport_height + 2.0) / 2.0 + camera_translation.y;
    auto camera_bound_top = (viewport_height + 2.0) / 2.0 + camera_translation.y;

    return camera_bound_left <= translation.x &&
           camera_bound_right >= translation.x &&
           camera_bound_bottom <= translation.y &&
           camera_bound_top >= translation.y;
  }

  Transform Camera::negateTransformForScreen(std::shared_ptr<Transform> trans) {
    //Gotta do this to make the camera move the opposite the renderables
    Transform negated_for_screen = *trans;
    negated_for_screen.translate(glm::vec3(-2.0, -2.0, 1.0) * negated_for_screen.getAbsoluteTranslation());
    return negated_for_screen;
  }
}